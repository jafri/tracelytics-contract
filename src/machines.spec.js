const expect = require('chai').expect;
const crypto = require('crypto');

const contract = "tracelytics"
const authorization = [{ actor: contract, permission: 'owner' }]
const company = "raptor"
const machineId = "SITE-1"
const name = "MACHINE NAME"
const machineChecksum = crypto.createHash('sha256').update(machineId).digest('hex')

const createMachineArgs = [
  {"key": "company",   "value": ["name", company]},
  {"key": "machineId", "value": ["string", machineId]},
  {"key": "name",      "value": ["string", name]}
]

const createMachineResult = {
    machine_checksum: machineChecksum,
    machine_id: machineId,
    name: name,
    site: "0000000000000000000000000000000000000000000000000000000000000000",
    data: []
}

const editMachineName = "NEW MACHINE NAME"
const editMachineArgs = [
  {"key": "company",   "value": ["name", company]},
  {"key": "machineId", "value": ["string", machineId]},
  {"key": "name",      "value": ["string", editMachineName]}
]
const editMachineResult = Object.assign({}, createMachineResult, {
  name: editMachineName
})

const delMachineArgs = [
  {"key": "company", "value": ["name", company]},
  {"key": "machineId",  "value": ["string", machineId]}
]

const getMachineRows = async () => {
  const { rows } = await getTable({
    code: contract,              // Contract that we target
    scope: company,              // Account that owns the data
    table: 'machine',               // Table name
    key_type: `sha256`,          // Type of key
    index_position: 2,           // Position of index
    lower_bound: machineChecksum,   // Table secondary key value
    limit: 1,                    // Here we limit to 1 to get only row
  });
  return rows
}

describe('Machines', () => {
  it('can create a machine', async () => {
    const action = createAction(contract, 'newmachine', { args: createMachineArgs }, authorization)
    const result = await transact([action])

    const machines = await getMachineRows()
    expect(machines).to.have.lengthOf(1)

    const machineRow = machines[0]
    delete machineRow.id
    expect(machineRow).to.deep.equal(createMachineResult)
  });

  it('can edit a machine', async () => {
    const action = createAction(contract, 'editmachine', { args: editMachineArgs }, authorization)
    const result = await transact([action])

    const rows = await getMachineRows()
    expect(rows).to.have.lengthOf(1)

    const machineRow = rows[0]
    delete machineRow.id
    expect(machineRow).to.deep.equal(editMachineResult)
  });

  it('can delete a machine', async () => {
    const action = createAction(contract, 'delmachine', { args: delMachineArgs }, authorization)
    const result = await transact([action])
    const rows = await getMachineRows()
    expect(rows).to.have.lengthOf(0)
  });
});