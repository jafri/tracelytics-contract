const expect = require('chai').expect;
const crypto = require('crypto');

const contract = "tracelytics"
const authorization = [{ actor: contract, permission: 'owner' }]
const company = "raptor"
const productinsId = "PRODUCT-1"
const productins = "PRODUCT NAME"
const productinsChecksum = crypto.createHash('sha256').update(productinsId).digest('hex')

const createProductInstanceArgs = [
  {"key": "company",      "value": ["name", company]},
  {"key": "productinsId", "value": ["string", productinsId]},
  {"key": "product",      "value": ["string", productins]}
]

const createProductInstanceResult = {
    productins_checksum: productinsChecksum,
    productins_id: productinsId,
    product: productins,
    site_checksum: "0000000000000000000000000000000000000000000000000000000000000000",
    site_id: "",
    data: []
}

const editProductInstanceName = "NEW PRODUCT NAME"
const editProductInstanceArgs = [
  {"key": "company",      "value": ["name", company]},
  {"key": "productinsId", "value": ["string", productinsId]},
  {"key": "product",      "value": ["string", editProductInstanceName]}
]
const editProductInstanceResult = Object.assign({}, createProductInstanceResult, {
  product: editProductInstanceName
})

const delProductInstanceArgs = [
  {"key": "company", "value": ["name", company]},
  {"key": "productinsId",  "value": ["string", productinsId]}
]

const getProductInstanceRows = async () => {
  const { rows } = await getTable({
    code: contract,              // Contract that we target
    scope: company,              // Account that owns the data
    table: 'productins',               // Table name
    key_type: `sha256`,          // Type of key
    index_position: 2,           // Position of index
    lower_bound: productinsChecksum,   // Table secondary key value
    limit: 1,                    // Here we limit to 1 to get only row
  });
  return rows
}

describe('ProductInstance Instances', () => {
  it('can create a productins instance', async () => {
    const action = createAction(contract, 'newprodins', { args: createProductInstanceArgs }, authorization)
    const result = await transact([action])

    const productins = await getProductInstanceRows()
    expect(productins).to.have.lengthOf(1)

    const productinsRow = productins[0]
    delete productinsRow.id
    expect(productinsRow).to.deep.equal(createProductInstanceResult)
  });

  it('can edit a productins instance', async () => {
    const action = createAction(contract, 'editprodins', { args: editProductInstanceArgs }, authorization)
    const result = await transact([action])

    const rows = await getProductInstanceRows()
    expect(rows).to.have.lengthOf(1)

    const productinsRow = rows[0]
    delete productinsRow.id
    expect(productinsRow).to.deep.equal(editProductInstanceResult)
  });

  it('can delete a productins instance', async () => {
    const action = createAction(contract, 'delprodins', { args: delProductInstanceArgs }, authorization)
    const result = await transact([action])
    const rows = await getProductInstanceRows()
    expect(rows).to.have.lengthOf(0)
  });
});