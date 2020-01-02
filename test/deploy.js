const fs = require(`fs`)
const path = require(`path`)
const { Serialize } = require(`@jafri/eosjs2`)

function getDeployableFilesFromDir(dir) {
  const dirCont = fs.readdirSync(dir)
  const wasmFileName = dirCont.find(filePath => filePath.match(/.*\.(wasm)$/gi))
  const abiFileName = dirCont.find(filePath => filePath.match(/.*\.(abi)$/gi))
  if (!wasmFileName) throw new Error(`Cannot find a ".wasm file" in ${dir}`)
  if (!abiFileName) throw new Error(`Cannot find an ".abi file" in ${dir}`)
  return {
    wasmPath: path.join(dir, wasmFileName),
    abiPath: path.join(dir, abiFileName),
  }
}

global.deployContract = async function deployContract(account, contractDir) {
  const { wasmPath, abiPath } = getDeployableFilesFromDir(contractDir)

  // 1. Prepare SETCODE
  // read the file and make a hex string out of it
  const wasm = fs.readFileSync(wasmPath).toString(`hex`)

  // 2. Prepare SETABI
  const buffer = new Serialize.SerialBuffer({
    textEncoder: eos.textEncoder,
    textDecoder: eos.textDecoder,
  })

  let abi = JSON.parse(fs.readFileSync(abiPath, `utf8`))
  const abiDefinition = eos.abiTypes.get(`abi_def`)
  // need to make sure abi has every field in abiDefinition.fields
  // otherwise serialize throws
  abi = abiDefinition.fields.reduce(
    (acc, { name: fieldName }) =>
      Object.assign(acc, { [fieldName]: acc[fieldName] || [] }),
    abi
  )
  abiDefinition.serialize(buffer, abi)

  // 3. Set code
  try {
    await transact([
      {
        account: 'eosio',
        name: 'setcode',
        authorization: [
          {
            actor: account,
            permission: 'active',
          },
        ],
        data: {
          account: account,
          vmtype: 0,
          vmversion: 0,
          code: wasm,
        },
      }
    ])
  } catch (e) {
    console.log('Set code failed')
  }

  // Set ABI
  try {
    await transact([
      {
        account: 'eosio',
        name: 'setabi',
        authorization: [
          {
            actor: account,
            permission: 'active',
          },
        ],
        data: {
          account: account,
          abi: Buffer.from(buffer.asUint8Array()).toString(`hex`),
        },
      }
    ])
  } catch (e) {
    console.log('Set abi failed')
  }
}

module.exports = {

}