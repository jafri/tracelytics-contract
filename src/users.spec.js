const expect = require('chai').expect;
const crypto = require('crypto');

const contract = "tracelytics"
const authorization = [{ actor: contract, permission: 'owner' }]
const company = "raptor"
const userId = "syed8"
const userChecksum = crypto.createHash('sha256').update(userId).digest('hex')
const key = "EOS5o31Jc1JRaCvRDWvWSEoHHwQBYdKLWCeUyCgHYwBatjcvXGkzo"
const permissions = "machine:create,machine:edit,machine:delete"

const createUserArgs = [
  {"key": "company", "value": ["name", company]},
  {"key": "userId", "value": ["string", userId]},
  {"key": "key", "value": ["public_key", key]},
  {"key": "permissions", "value": ["string", permissions]}
]
const createUserResult = {
    user_checksum: userChecksum,
    user_id: userId,
    key: key,
    nonce: 0,
    permissions: permissions.split(","),
    data: []
}

const editedPermissions = "user:create"
const editUserArgs = [
  {"key": "company", "value": ["name", company]},
  {"key": "userId", "value": ["string", userId]},
  {"key": "permissions", "value": ["string", "user:create"]}
]
const editUserResult = Object.assign({}, createUserResult, { permissions: editedPermissions.split(",") })

const delUserArgs = [
  {"key": "company", "value": ["name", company]},
  {"key": "userId", "value": ["string", userId]}
]

const getUserRows = async () => {
    const { rows } = await getTable({
        code: contract,             // Contract that we target
        scope: company,             // Account that owns the data
        table: 'user',              // Table name
        key_type: `sha256`,         // Type of key
        index_position: 2,          // Position of index
        lower_bound: userChecksum,  // Table secondary key value
        limit: 1,                   // Here we limit to 1 to get only row
    });
    return rows
}

describe('Users', () => {
  it('can create a user', async () => {
    const action = createAction(contract, 'newuser', { args: createUserArgs }, authorization)
    const result = await transact([action])

    const users = await getUserRows()
    expect(users).to.have.lengthOf(1)

    const userRow = users[0]
    delete userRow.id
    expect(userRow).to.deep.equal(createUserResult)
  });

  it('can edit a user', async () => {
    const action = createAction(contract, 'edituser', { args: editUserArgs }, authorization)
    const result = await transact([action])

    const rows = await getUserRows()
    expect(rows).to.have.lengthOf(1)

    const userRow = rows[0]
    delete userRow.id
    expect(userRow).to.deep.equal(editUserResult)
  });

  it('can delete a user', async () => {
    const action = createAction(contract, 'deluser', { args: delUserArgs }, authorization)
    const result = await transact([action])
    const rows = await getUserRows()
    expect(rows).to.have.lengthOf(0)
  });
});