require("./helper")
require("./deploy")

const newaccount = "tracelytics"

const main = async () => {
  try {
    const account = await eos.rpc.get_account(newaccount);
  } catch (e) {
    await transact([
      {
        account: 'eosio',
        name: 'newaccount',
        data: {
          owner: {
            accounts: [],
            keys: [
              { key: "EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV", weight: 1 }
            ],
            threshold: 1,
            waits: []
          },
          active: {
            accounts: [
              {
                permission: {
                  actor: 'tracelytics',
                  permission: 'eosio.code'
                },
                weight: 1
              }
            ],
            keys: [
              { key:"EOS6MRyAjQq8ud7hVNYcfnVPJqcVpscN5So8BhtHuGYqET5GDW5CV", weight: 1 }
            ],
            threshold: 1,
            waits: []
          },
          creator: "eosio",
          name: newaccount
        },
        authorization: [{ actor: 'eosio', permission: 'active' }]
      }
    ])
  }

  console.log('deploying')
  await deployContract(newaccount, __dirname + '/../')
}

main()