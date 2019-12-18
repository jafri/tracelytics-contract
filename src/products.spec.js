const expect = require('chai').expect;
const crypto = require('crypto');

const contract = "tracelytics"
const authorization = [{ actor: contract, permission: 'owner' }]
const company = "raptor"
const productId = "PRODUCT-1"
const name = "PRODUCT NAME"
const description = ""
const productChecksum = crypto.createHash('sha256').update(productId).digest('hex')

const createProductArgs = [
  {"key": "company",   "value": ["name", company]},
  {"key": "productId", "value": ["string", productId]},
  {"key": "name",      "value": ["string", name]}
]

const createProductResult = {
    product_checksum: productChecksum,
    product_id: productId,
    name: name,
    description: '',
    data: []
}

const editProductName = "NEW PRODUCT NAME"
const editProductDescription = "NEW PRODUCT DESCRIPTION"
const editProductArgs = [
  {"key": "company",     "value": ["name", company]},
  {"key": "productId",   "value": ["string", productId]},
  {"key": "name",        "value": ["string", editProductName]},
  {"key": "description", "value": ["string", editProductDescription]}
]
const editProductResult = Object.assign({}, createProductResult, {
  name: editProductName,
  description: editProductDescription
})

const delProductArgs = [
  {"key": "company", "value": ["name", company]},
  {"key": "productId",  "value": ["string", productId]}
]

const getProductRows = async () => {
  const { rows } = await getTable({
    code: contract,              // Contract that we target
    scope: company,              // Account that owns the data
    table: 'product',               // Table name
    key_type: `sha256`,          // Type of key
    index_position: 2,           // Position of index
    lower_bound: productChecksum,   // Table secondary key value
    limit: 1,                    // Here we limit to 1 to get only row
  });
  return rows
}

describe('Products', () => {
  it('can create a product', async () => {
    const action = createAction(contract, 'newproduct', { args: createProductArgs }, authorization)
    const result = await transact([action])

    const products = await getProductRows()
    expect(products).to.have.lengthOf(1)

    const productRow = products[0]
    delete productRow.id
    expect(productRow).to.deep.equal(createProductResult)
  });

  it('can edit a product', async () => {
    const action = createAction(contract, 'editproduct', { args: editProductArgs }, authorization)
    const result = await transact([action])

    const rows = await getProductRows()
    expect(rows).to.have.lengthOf(1)

    const productRow = rows[0]
    delete productRow.id
    expect(productRow).to.deep.equal(editProductResult)
  });

  it('can delete a product', async () => {
    const action = createAction(contract, 'delproduct', { args: delProductArgs }, authorization)
    const result = await transact([action])
    const rows = await getProductRows()
    expect(rows).to.have.lengthOf(0)
  });
});