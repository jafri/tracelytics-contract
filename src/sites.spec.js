const expect = require('chai').expect;
const crypto = require('crypto');

const contract = "tracelytics"
const authorization = [{ actor: contract, permission: 'owner' }]
const company = "raptor"
const siteId = "SITE-1"
const name = "SITE NAME"
const siteChecksum = crypto.createHash('sha256').update(siteId).digest('hex')

const createSiteArgs = [
  {"key": "company", "value": ["name", company]},
  {"key": "siteId",  "value": ["string", siteId]},
  {"key": "name",    "value": ["string", name]}
]

const createSiteResult = {
    site_checksum: siteChecksum,
    site_id: siteId,
    name: name,
    data: []
}

const editSiteName = "NEW SITE NAME"
const editSiteArgs = [
  {"key": "company", "value": ["name", company]},
  {"key": "siteId",  "value": ["string", siteId]},
  {"key": "name",    "value": ["string", editSiteName]}
]
const editSiteResult = Object.assign({}, createSiteResult, {
  name: editSiteName
})

const delSiteArgs = [
  {"key": "company", "value": ["name", company]},
  {"key": "siteId",  "value": ["string", siteId]}
]

const getSiteRows = async () => {
  const { rows } = await getTable({
    code: contract,              // Contract that we target
    scope: company,              // Account that owns the data
    table: 'site',               // Table name
    key_type: `sha256`,          // Type of key
    index_position: 2,           // Position of index
    lower_bound: siteChecksum,   // Table secondary key value
    limit: 1,                    // Here we limit to 1 to get only row
  });
  return rows
}

describe('Sites', () => {
  it('can create a site', async () => {
    const action = createAction(contract, 'newsite', { args: createSiteArgs }, authorization)
    const result = await transact([action])

    const sites = await getSiteRows()
    expect(sites).to.have.lengthOf(1)

    const siteRow = sites[0]
    delete siteRow.id
    expect(siteRow).to.deep.equal(createSiteResult)
  });

  it('can edit a site', async () => {
    const action = createAction(contract, 'editsite', { args: editSiteArgs }, authorization)
    const result = await transact([action])

    const rows = await getSiteRows()
    expect(rows).to.have.lengthOf(1)

    const siteRow = rows[0]
    delete siteRow.id
    expect(siteRow).to.deep.equal(editSiteResult)
  });

  it('can delete a site', async () => {
    const action = createAction(contract, 'delsite', { args: delSiteArgs }, authorization)
    const result = await transact([action])
    const rows = await getSiteRows()
    expect(rows).to.have.lengthOf(0)
  });
});