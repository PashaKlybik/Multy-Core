/* Copyright 2019 by Multy.io
 * Licensed under Multy.io license.
 *
 * See LICENSE for details
 */

#include "multy_core/src/ethereum/ethereum_transaction_builder_bloqboard.h"

#include "multy_core/src/ethereum/ethereum_account.h"
#include "multy_core/src/ethereum/ethereum_address.h"
#include "multy_core/src/ethereum/ethereum_stream.h"
#include "multy_core/src/ethereum/ethereum_transaction.h"
#include "multy_core/src/ethereum/ethereum_transaction_builder.h"

#include "multy_core/src/api/transaction_builder_impl.h"
#include "multy_core/src/api/properties_impl.h"
#include "multy_core/src/api/big_int_impl.h"
#include "multy_core/src/property_predicates.h"
#include "multy_core/src/utility.h"

#include <string>
#include <unordered_map>

#include <iostream>

namespace
{

using namespace multy_core::internal;

class BLOQBOARDTransactionBuilderBase : public TransactionBuilder
{
protected:
    const EthereumAccount& m_account;
    Properties m_properties;
    PropertyT<BigInt> m_sender_eth_balance;
    FunctionalPropertyT<EthereumAddress, std::string> m_contract_address;

    BLOQBOARDTransactionBuilderBase(const EthereumAccount& account, const std::string& name)
        : m_account(account),
          m_properties(ERROR_SCOPE_TRANSACTION_BUILDER, "BLOQBOARD/" + name),
          m_sender_eth_balance(
                  m_properties,
                  "balance_eth",
                  Property::REQUIRED,
                  verify_in_range(0, ValueRangeChecker::NoBound)),
          m_contract_address(
                  m_properties,
                  "contract_address",
                  &EthereumAddress::from_string,
                  &EthereumAddress::to_string)
    {}
    ~BLOQBOARDTransactionBuilderBase()
    {}

    TransactionPtr make_transaction() const override
    {
        validate();

        TransactionPtr transaction(new EthereumTransaction(m_account));
        {
            Properties& source = transaction->add_source();
            source.set_property_value("amount", *m_sender_eth_balance);
        }

        {
            Properties& destination = transaction->add_destination();
            destination.set_property_value("address", EthereumAddress::to_string(*m_contract_address));
            destination.set_property_value("amount", BigInt(0));
        }

        transaction->set_message(*make_message());

        return transaction;
    }
    Properties& get_properties() override
    {
        return m_properties;
    }
    void validate() const override
    {
        m_properties.validate(MULTY_CODE_LOCATION);
    }

    virtual BinaryDataPtr make_message() const = 0;
};

// mint(uint256 _amount)
class BLOQBOARDMintTransactionBuilder : public BLOQBOARDTransactionBuilderBase
{
private:
    PropertyT<BigInt> m_balance_token;
    PropertyT<BigInt> m_amount_token;

public:
    BLOQBOARDMintTransactionBuilder(const EthereumAccount& account, const std::string& name)
        : BLOQBOARDTransactionBuilderBase(account, name),
          m_balance_token(m_properties,
              "balance_token",
              Property::REQUIRED,
              &verify_bigger_than<BigInt, 0>),
          m_amount_token(m_properties,
              "mint_amount_token",
              Property::REQUIRED,
              &verify_bigger_than<BigInt, 0>)
    {}

    BinaryDataPtr make_message() const override
    {
        const static auto methodSig = make_method_hash("mint(uint256)");
        for (int var = 0; var < 4; ++var) {
            std::cout << methodSig[var];
        }

        EthereumPayloadDataStream result;
        EthereumPayloadAsUint256Stream variables;
        result << methodSig;
        variables << *m_amount_token;
        result << variables;

        return make_clone(result.get_content());
    }
};

// Not part of official BLOQBOARD standard, but common enough, an ERC20-style Redeem:
// Redeem(uint256 _amount)
class BLOQBOARDRedeemTransactionBuilder : public BLOQBOARDTransactionBuilderBase
{
private:
    PropertyT<BigInt> m_mint_token;
    PropertyT<BigInt> m_amount_token;
public:
    BLOQBOARDRedeemTransactionBuilder(const EthereumAccount& account, const std::string& name)
        : BLOQBOARDTransactionBuilderBase(account, name),
          m_mint_token(m_properties,
              "mint_token",
              Property::REQUIRED,
              &verify_bigger_than<BigInt, 0>),
          m_amount_token(m_properties,
              "redeem_amount_token",
              Property::REQUIRED,
              &verify_bigger_than<BigInt, 0>)
    {}

    BinaryDataPtr make_message() const override
    {
        const static auto methodSig = make_method_hash("redeem(uint256)");

        EthereumPayloadDataStream result;
        EthereumPayloadAsUint256Stream variables;
        result << methodSig;
        variables << *m_amount_token;
        result << variables;

        return make_clone(result.get_content());
    }
};


// Official BLOQBOARD standard
// redeemUnderlying(uint256 _amount)
class BLOQBOARDRedeemUnderlyingTransactionBuilder : public BLOQBOARDTransactionBuilderBase
{
private:
    PropertyT<BigInt> m_mint_token;
    PropertyT<BigInt> m_amount_token;
public:
    BLOQBOARDRedeemUnderlyingTransactionBuilder(const EthereumAccount& account, const std::string& name)
        : BLOQBOARDTransactionBuilderBase(account, name),
          m_mint_token(m_properties,
              "mint_token",
              Property::REQUIRED,
              &verify_bigger_than<BigInt, 0>),
          m_amount_token(m_properties,
              "redeem_amount_token",
              Property::REQUIRED,
              &verify_bigger_than<BigInt, 0>)
    {}

    BinaryDataPtr make_message() const override
    {
        const static auto methodSig = make_method_hash("redeemUnderlying(uint256)");

        EthereumPayloadDataStream result;
        EthereumPayloadAsUint256Stream variables;
        result << methodSig;
        variables << *m_amount_token;
        result << variables;

        return make_clone(result.get_content());
    }
};

} // namespace

namespace multy_core
{
namespace internal
{

TransactionBuilderPtr make_ethereum_bloqboard_transaction_builder(
        const EthereumAccount& account,
        const std::string& action)
{
    typedef TransactionBuilder* (*BuilderFunction)(const EthereumAccount&, const std::string&);
    static const std::unordered_map<std::string, BuilderFunction> BUILDERS =
    {
        {"mint",  &new_tx_builder<BLOQBOARDMintTransactionBuilder>},
        {"redeem",  &new_tx_builder<BLOQBOARDRedeemTransactionBuilder>},
        {"redeemUnderlying", &new_tx_builder<BLOQBOARDRedeemUnderlyingTransactionBuilder>}
    };

    const auto builder = BUILDERS.find(action);
    if (builder == BUILDERS.end())
    {
        THROW_EXCEPTION2(ERROR_INVALID_ARGUMENT,
                "Invalid Ethereum BLOQBOARD TransactionBuilder.")
                << " action: \"" << action << "\".";
    }

    return TransactionBuilderPtr(builder->second(account, action));
}

} // namespace internal
} // namespace multy_core
