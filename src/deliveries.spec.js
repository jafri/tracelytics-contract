const expect = require('chai').expect;
const crypto = require('crypto');

const contract = "tracelytics"
const authorization = [{ actor: contract, permission: 'owner' }]

// Create
const company = "raptor"
const deliveryId = "DEL-1"
const driverId = "DRI-1"
const shipperId = "UPS"
const startTime = "2019-01-02"
const status = "enroute"
const deliveryChecksum = crypto.createHash('sha256').update(deliveryId).digest('hex')

const createDeliveryArgs = [
  {"key": "company",    "value": ["name", company]},
  {"key": "deliveryId", "value": ["string", deliveryId]},
  {"key": "shipperId",  "value": ["string", shipperId]},
  {"key": "driverId",   "value": ["string", driverId]},
  {"key": "startTime",  "value": ["string", startTime]},
  {"key": "status",     "value": ["string", status]}
]

const createDeliveryResult = {
    delivery_checksum: deliveryChecksum,
    delivery_id: deliveryId,
    shipper_id: shipperId,
    driver_id: driverId,
    start_time: startTime,
    end_time: '',
    status: status,
    cargo: [{
      key: "bae02da828e9f38bfc1b97cd6188b861bdc6d5d2f3ee123be91c32d1930524ea",
      value: {
        delta: 0,
        product_checksum: "cf169cabe865f2c99cc40cebf109c276836d7a7793b226949245519ea8040542",
        quantity: 4
      }
    }],
    data: []
}

// Edit
const editDeliveryEndTime = "2019-01-05"
const editDeliveryStatus = "delivered"
const editDeliveryArgs = [
  {"key": "company",    "value": ["name", company]},
  {"key": "deliveryId", "value": ["string", deliveryId]},
  {"key": "endTime",    "value": ["string", editDeliveryEndTime]},
  {"key": "status",     "value": ["string", editDeliveryStatus]}
]
const editDeliveryResult = Object.assign({}, createDeliveryResult, {
  end_time: editDeliveryEndTime,
  status: editDeliveryStatus
})

// Delete
const delDeliveryArgs = [
  {"key": "company",    "value": ["name", company]},
  {"key": "deliveryId", "value": ["string", deliveryId]}
]

// Cargo
const productinsId = "PRODUCT-1"
const productins = "PRODUCT NAME"
const cargo = [{
  key: crypto.createHash('sha256').update(productinsId).digest('hex'),
  value: {
    product_checksum: crypto.createHash('sha256').update(productins).digest('hex'),
    quantity: 4,
    delta: 0
  }
}]

const getDeliveryRows = async () => {
    const { rows } = await getTable({
        code: contract,                // Contract that we target
        scope: company,                // Account that owns the data
        table: 'delivery',             // Table name
        key_type: `sha256`,            // Type of key
        index_position: 2,             // Position of index
        lower_bound: deliveryChecksum, // Table secondary key value
        limit: 1,                      // Here we limit to 1 to get only row
    });
    return rows
}

describe('Deliveries', () => {
  it('can create a delivery', async () => {
    const action = createAction(contract, 'newdelivery', { args: createDeliveryArgs, cargo }, authorization)
    const result = await transact([action])

    const deliveries = await getDeliveryRows()
    expect(deliveries).to.have.lengthOf(1)

    const deliveryRow = deliveries[0]
    delete deliveryRow.id
    expect(deliveryRow).to.deep.equal(createDeliveryResult)
  });

  it('can edit a delivery', async () => {
    const action = createAction(contract, 'editdelivery', { args: editDeliveryArgs, cargo: [] }, authorization)
    const result = await transact([action])

    const rows = await getDeliveryRows()
    expect(rows).to.have.lengthOf(1)

    const deliveryRow = rows[0]
    delete deliveryRow.id
    expect(deliveryRow).to.deep.equal(editDeliveryResult)
  });

  // it('can delete a delivery', async () => {
  //   const action = createAction(contract, 'deldelivery', { args: delDeliveryArgs }, authorization)
  //   const result = await transact([action])
  //   const rows = await getDeliveryRows()
  //   expect(rows).to.have.lengthOf(0)
  // });
});