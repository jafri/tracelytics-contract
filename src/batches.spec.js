const expect = require('chai').expect;
const crypto = require('crypto');

const contract = "tracelytics"
const authorization = [{ actor: contract, permission: 'owner' }]
const company = "raptor"
const userId = "syed1"
const batchId = "CHE-1"
const batchChecksum = crypto.createHash('sha256').update(batchId).digest('hex')
const machineId = "MACH-1"
const date = "2019-01-02"

const createBatchArgs = [
  {"key": "company",   "value": ["name", company]},
  {"key": "batchId",   "value": ["string", batchId]},
  {"key": "userId",    "value": ["string", userId]},
  {"key": "machineId", "value": ["string", machineId]},
  {"key": "date",      "value": ["string", date]},
]
const createBatchResult = {
    batch_checksum: batchChecksum,
    batch_id: batchId,
    user_id: userId,
    machine_id: machineId,
    date: date,
    data: []
}

const editBatchUserId = "syed2"
const editBatchArgs = [
  {"key": "company", "value": ["name", company]},
  {"key": "batchId", "value": ["string", batchId]},
  {"key": "userId",  "value": ["string", editBatchUserId]}
]
const editBatchResult = Object.assign({}, createBatchResult, {
    user_id: editBatchUserId
})

const delBatchArgs = [
  {"key": "company", "value": ["name", company]},
  {"key": "batchId", "value": ["string", batchId]}
]

const getBatchRows = async () => {
    const { rows } = await getTable({
        code: contract,             // Contract that we target
        scope: company,             // Account that owns the data
        table: 'batch',             // Table name
        key_type: `sha256`,         // Type of key
        index_position: 2,          // Position of index
        lower_bound: batchChecksum, // Table secondary key value
        limit: 1,                   // Here we limit to 1 to get only row
    });
    return rows
}

describe('Batches', () => {
  it('can create a batch', async () => {
    const action = createAction(contract, 'newbatch', { args: createBatchArgs }, authorization)
    const result = await transact([action])

    const batches = await getBatchRows()
    expect(batches).to.have.lengthOf(1)

    const batchRow = batches[0]
    delete batchRow.id
    expect(batchRow).to.deep.equal(createBatchResult)
  });

  it('can edit a batch', async () => {
    const action = createAction(contract, 'editbatch', { args: editBatchArgs }, authorization)
    const result = await transact([action])

    const rows = await getBatchRows()
    expect(rows).to.have.lengthOf(1)

    const batchRow = rows[0]
    delete batchRow.id
    expect(batchRow).to.deep.equal(editBatchResult)
  });

  it('can delete a batch', async () => {
    const action = createAction(contract, 'delbatch', { args: delBatchArgs }, authorization)
    const result = await transact([action])
    const rows = await getBatchRows()
    expect(rows).to.have.lengthOf(0)
  });
});