/* Copyright 2018 by Multy.io
 * Licensed under Multy.io license.
 *
 * See LICENSE for details
 */

#include "multy_core/src/ethereum/ethereum_facade.h"

#include "multy_core/ethereum.h"
#include "multy_core/src/ethereum/ethereum_account.h"
#include "multy_core/src/ethereum/ethereum_address.h"
#include "multy_core/src/ethereum/ethereum_transaction.h"
#include "multy_core/src/ethereum/ethereum_transaction_builder_multisig.h"
#include "multy_core/src/ethereum/ethereum_transaction_builder_erc20.h"
#include "multy_core/src/ethereum/ethereum_transaction_builder_erc721.h"
#include "multy_core/src/ethereum/ethereum_transaction_builder.h"

#include "multy_core/src/exception.h"
#include "multy_core/src/exception_stream.h"
#include "multy_core/src/enum_name_map.h"

#include "multy_core/src/codec.h"

#include <unordered_map>

namespace
{

void validate_ethereum_account_type(uint32_t account_type)
{
    if (account_type != 0)
    {
        THROW_EXCEPTION2(ERROR_INVALID_ARGUMENT,
                "Unknown Ethereum account type.")
                << " Value: " << account_type << ".";
    }
}

} // namespace

namespace multy_core
{
namespace internal
{

EthereumFacade::EthereumFacade()
{
}

EthereumFacade::~EthereumFacade()
{
}

HDAccountPtr EthereumFacade::make_hd_account(
        BlockchainType blockchain_type,
        uint32_t account_type,
        const ExtendedKey& master_key,
        uint32_t index) const
{
    validate_ethereum_account_type(account_type);

    return HDAccountPtr(new EthereumHDAccount(blockchain_type, master_key, index));
}

AccountPtr EthereumFacade::make_account(
        BlockchainType blockchain_type,
        uint32_t account_type,
        const char* serialized_private_key) const
{
    validate_ethereum_account_type(account_type);

    return AccountPtr(make_ethereum_account(blockchain_type, serialized_private_key).release());
}

TransactionPtr EthereumFacade::make_transaction(const Account& account) const
{
    return TransactionPtr(new EthereumTransaction(account));
}

TransactionBuilderPtr EthereumFacade::make_transaction_builder(
        const Account& account, uint32_t type, const char* action) const
{
    typedef TransactionBuilderPtr (*BuilderFactoryFunction)(const EthereumAccount&, const std::string&);
    static const std::unordered_map<size_t, BuilderFactoryFunction> BUILDERS =
    {
        {
            ETHEREUM_TRANSACTION_BUILDER_MULTISIG,
            &make_ethereum_multisig_transaction_builder
        },
        {
            ETHEREUM_TRANSACTION_BUILDER_ERC20,
            &make_ethereum_ERC20_transaction_builder
        },
        {
            ETHEREUM_TRANSACTION_BUILDER_BASIC,
            &make_ethereum_transaction_builder
        },
        {
            ETHEREUM_TRANSACTION_BUILDER_ERC721,
            &make_ethereum_ERC721_transaction_builder
        },
    };

    const auto builder = BUILDERS.find(type);
    if (builder == BUILDERS.end())
    {
        THROW_EXCEPTION2(ERROR_INVALID_ARGUMENT,
                "Invalid TransactionBuilder type.")
                << " Type: " << type
                << ", action: \"" << (action ? action : "") << "\".";
    }

    INVARIANT(account.get_blockchain_type().blockchain == BLOCKCHAIN_ETHEREUM);
    const auto& ethereum_account = dynamic_cast<const EthereumAccount&>(account);

    return builder->second(ethereum_account, std::string(action ? action : ""));
}

TransactionBuilderPtr EthereumFacade::make_transaction_builder_by_name(
        const Account& account,
        const char* name,
        const char* action) const
{
    static const EnumNameMap<EthereumTransactionBuilderType> Builders =
    {
        "Ethereum transaction builder",
        {
            { ETHEREUM_TRANSACTION_BUILDER_ERC20, "erc20" },
            { ETHEREUM_TRANSACTION_BUILDER_MULTISIG, "multisig" },
            { ETHEREUM_TRANSACTION_BUILDER_BASIC, "basic" },
            { ETHEREUM_TRANSACTION_BUILDER_ERC721, "erc721" },
        }
    };

    return make_transaction_builder(account, Builders.get_value(name), action);
}

void EthereumFacade::validate_address(
        BlockchainType, const char* address) const
{
    EthereumAddress::from_string(address);
}

std::string EthereumFacade::encode_serialized_transaction(
        Transaction* transaction) const
{
    INVARIANT(transaction != nullptr);
    const BinaryDataPtr serialized_transaction = transaction->serialize();
    return "0x" + encode(*serialized_transaction, CODEC_HEX);
}

} // namespace internal
} // namespace multy_core
