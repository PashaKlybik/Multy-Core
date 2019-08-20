/* Copyright 2019 by Multy.io
 * Licensed under Multy.io license.
 *
 * See LICENSE for details
 */

#include "test_json_api_transaction.h"

namespace
{
using namespace test_utility;


const JsonTransactionApiTestCase ETH_BLOQBOARD_TEST_CASES[] = {
    // BLOQBOARD mint
    {
        R"json({
    "blockchain": "Ethereum",
    "net_type": 1,
    "account": {
        "type": 0,
        "private_key": "e5fbc2dd65f5cffa6a5e7d4d7069c574eb0702597d3f6941427d453658e0827f"
    },
    "builder": {
        "type": "bloqboard",
        "action": "mint",
        "payload": {
            "balance_eth": "100000000000000000",
            "contract_address": "0x39aa39c021dfbae8fac545936693ac917d5e7563",
            "balance_token": "1000000000000000000",
            "mint_amount_token": "5"
        }
    },
    "transaction": {
        "nonce": 3,
        "fee": {
            "gas_price": "5000000000",
            "gas_limit": "500000"
        }
    }
})json",
        // TX id: 0xdfbaf91c0035e0442bff8226d33df46ceb10a1a278da14df3dfd1d9f46ab5fe1
        TX("0xf8890385012a05f2008307a1209439aa39c021dfbae8fac545936693ac917d5e756380a4a0712d6"
        "8000000000000000000000000000000000000000000000000000000000000000525a03fea9a4da1260ca"
        "442757cd0e457906ecc514d29e7f53a209855bfe890f4fcc4a001dec14569b67cb252f2cab14915c5391"
        "65f7bc0b9d4cb82d8551f64532611a7")
    },
    // BLOQBOARD redeem
    {
        R"json({
    "blockchain": "Ethereum",
    "net_type": 1,
    "account": {
        "type": 0,
        "private_key": "e5fbc2dd65f5cffa6a5e7d4d7069c574eb0702597d3f6941427d453658e0827f"
    },
    "builder": {
        "type": "bloqboard",
        "action": "redeem",
        "payload": {
            "balance_eth": "100000000000000000",
            "contract_address": "0x39aa39c021dfbae8fac545936693ac917d5e7563",
            "mint_token": "100000",
            "redeem_amount_token": "5"
        }
    },
    "transaction": {
        "nonce": 4,
        "fee": {
            "gas_price": "6000000000",
            "gas_limit": "500000"
        }
    }
})json",
        // TX id: 0x524e87a8aaa013403fbc9ef39199f963ee4af70e8a125ebcc7832e65fa7d760b
        TX("0xf88904850165a0bc008307a1209439aa39c021dfbae8fac545936693ac917d5e756380a4db006a7"
        "5000000000000000000000000000000000000000000000000000000000000000525a0d34ee414f264601"
        "e4ddcd065cdb26b3e0abfc3250342265eb6ff6c3369c489d3a03b8425c1bd53935bfd5a2912ae566a7fd"
        "8bab8b3aaef674f152280b13ff9d8d0")
    },
    // BLOQBOARD redeem
    {
        R"json({
    "blockchain": "Ethereum",
    "net_type": 1,
    "account": {
        "type": 0,
        "private_key": "e5fbc2dd65f5cffa6a5e7d4d7069c574eb0702597d3f6941427d453658e0827f"
    },
    "builder": {
        "type": "bloqboard",
        "action": "redeem",
        "payload": {
            "balance_eth": "100000000000000000",
            "contract_address": "0x39aa39c021dfbae8fac545936693ac917d5e7563",
            "mint_token": "100000",
            "redeem_amount_token": "5"
        }
    },
    "transaction": {
        "nonce": 4,
        "fee": {
            "gas_price": "6000000000",
            "gas_limit": "500000"
        }
    }
})json",
        // TX id: 0x524e87a8aaa013403fbc9ef39199f963ee4af70e8a125ebcc7832e65fa7d760b
        TX("0xf88904850165a0bc008307a1209439aa39c021dfbae8fac545936693ac917d5e756380a4db006a7"
        "5000000000000000000000000000000000000000000000000000000000000000525a0d34ee414f264601"
        "e4ddcd065cdb26b3e0abfc3250342265eb6ff6c3369c489d3a03b8425c1bd53935bfd5a2912ae566a7fd"
        "8bab8b3aaef674f152280b13ff9d8d0")
    },
    {
        R"json({
    "blockchain": "Ethereum",
    "net_type": 1,
    "account": {
        "type": 0,
        "private_key": "e5fbc2dd65f5cffa6a5e7d4d7069c574eb0702597d3f6941427d453658e0827f"
    },
    "builder": {
        "type": "erc20",
        "action": "approve",
        "payload": {
            "balance_eth": "100000000000000000",
            "contract_address": "0xA0b86991c6218b36c1d19D4a2e9Eb0cE3606eB48",
            "balance_token": "100000",
            "address_for_approve": "0x39aa39c021dfbae8fac545936693ac917d5e7563",
            "approve_amount_token": "5"
        }
    },
    "transaction": {
        "nonce": 2,
        "fee": {
            "gas_price": "5000000000",
            "gas_limit": "100000"
        }
    }
})json",
        // TX id: 0xec5022503d57cbf963f0280e85f6bd943bbd5a7bdd5fc6e6a454383895cc3c47
        TX("0xf8aa0285012a05f200830186a094a0b86991c6218b36c1d19d4a2e9eb0ce3606eb4880b844095ea"
        "7b300000000000000000000000039aa39c021dfbae8fac545936693ac917d5e756300000000000000000"
        "0000000000000000000000000000000000000000000000526a050d063fd7e06b246f275bef1bdef5b7b4"
        "04ad24bb1696554b0ddb8276e089de4a01a5f66b80d1a6162d4f5bdc4634788c195af57d391c8cc980dc"
        "a372a85355202")
    },
    // BLOQBOARD redeem
    {
        R"json({
    "blockchain": "Ethereum",
    "net_type": 1,
    "account": {
        "type": 0,
        "private_key": "e5fbc2dd65f5cffa6a5e7d4d7069c574eb0702597d3f6941427d453658e0827f"
    },
    "builder": {
        "type": "bloqboard",
        "action": "redeemUnderlying",
        "payload": {
            "balance_eth": "100000000000000000",
            "contract_address": "0x39aa39c021dfbae8fac545936693ac917d5e7563",
            "mint_token": "100000",
            "redeem_amount_token": "5"
        }
    },
    "transaction": {
        "nonce": 6,
        "fee": {
            "gas_price": "3000000000",
            "gas_limit": "500000"
        }
    }
})json",
        // TX id: 0x524e87a8aaa013403fbc9ef39199f963ee4af70e8a125ebcc7832e65fa7d760b
        TX("0xf8880684b2d05e008307a1209439aa39c021dfbae8fac545936693ac917d5e756380a4852a12e30"
        "00000000000000000000000000000000000000000000000000000000000000525a035be0229cf85aaaaa"
        "7ecbfa989e9fde4acd6ac1f70fcf271db08f7a16a7707aea026620aa9da3e2cfa0d9a3048efce54e361e"
        "efa550012b955535971fcc9bbcefd")
    },
};


INSTANTIATE_TEST_CASE_P(
        EthereumBLOQBOARD,
        JsonTransactionApiTestP,
        ::testing::ValuesIn(ETH_BLOQBOARD_TEST_CASES));


} // unnamed namespace
