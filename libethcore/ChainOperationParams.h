/*
	This file is part of cpp-ethereum.

	cpp-ethereum is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	cpp-ethereum is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with cpp-ethereum.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file ChainOperationsParams.h
 * @author Gav Wood <i@gavwood.com>
 * @date 2015
 */

#pragma once

#include <libdevcore/Common.h>
#include "Common.h"
#include <libevmcore/EVMSchedule.h>

#include <vector>

namespace dev
{
namespace eth
{


class PrecompiledContract
{
public:
	PrecompiledContract() = default;
	PrecompiledContract(std::function<bigint(unsigned)> const& _cost, std::function<void(bytesConstRef, bytesRef)> const& _exec):
		m_cost(_cost),
		m_execute(_exec)
	{}
	PrecompiledContract(unsigned _base, unsigned _word, std::function<void(bytesConstRef, bytesRef)> const& _exec);

	bigint cost(bytesConstRef _in) const { return m_cost(_in.size()); }
	void execute(bytesConstRef _in, bytesRef _out) const { m_execute(_in, _out); }

private:
	std::function<bigint(unsigned)> m_cost;
	std::function<void(bytesConstRef, bytesRef)> m_execute;
};

struct ChainOperationParams
{
	ChainOperationParams();

	explicit operator bool() const { return accountStartNonce != Invalid256; }

	/// The chain sealer name: e.g. PBFT, Raft, SinglePoint
	std::string sealEngineName = "PBFT";  /* 共识算法（可选PBFT、RAFT、SinglePoint） */

	/// General chain params.
	u256 blockReward = 0;
	u256 maximumExtraDataSize = 1024;
	u256 accountStartNonce = 0;
	bool tieBreakingGas = true;

	/// Precompiled contracts as specified in the chain params.
	std::unordered_map<Address, PrecompiledContract> precompiled;

	/**
	 * @brief Additional parameters.
	 *
	 * e.g. Ethash specific:
	 * - minGasLimit
	 * - maxGasLimit
	 * - gasLimitBoundDivisor
	 * - minimumDifficulty
	 * - difficultyBoundDivisor
	 * - durationLimit
	 */
	std::unordered_map<std::string, std::string> otherParams;

	/// Convenience method to get an otherParam as a u256 int.
	u256 u256Param(std::string const& _name) const;
	std::string param(std::string const& _name) const;

	bool evmEventLog = false; 
	bool evmCoverLog = false; 
	bool statLog = false; 
	Address sysytemProxyAddress; /* 系统路由合约地址 */
	Address god; /* 内置链管理员账号地址 */

	std::string listenIp; /* 节点监听IP */
	int cryptoMod = 0;  /* 加密模式默认为0（与cryptomod.json文件中cryptomod字段保持一致  ）*/
	int cryptoprivatekeyMod = 0;//0：no keycenter 1：keycenter
	int ssl = 0;
	int rpcPort = 6789; /* RPC监听端口 */
	int rpcSSLPort = 6790;
	int p2pPort = 16789; /* P2P网络监听端口 */
	std::string wallet;  /* 钱包文件路径 */
	std::string keystoreDir; /* 账号文件目录路径 */
	std::string dataDir; /* 节点数据目录路径 */
	std::string logFileConf; /* 日志配置文件路径 */
	
	//dfs related parameters
	std::string nodeId;
	std::string groupId;
	std::string storagePath;
	
	std::string rateLimitConfig;
	int statsInterval;
	int channelPort = 0; /* 链上链下监听端口 */

	std::string vmKind; /* vm引擎 （默认 interpreter ） */
	unsigned networkId;
	int logVerbosity = 4; /* 日志级别（级别越高日志越详细，>8 TRACE日志，4<=x<8 DEBU G日志，<4 
	INFO日志 */

	u256 intervalBlockTime = 3000;

	bool broadcastToNormalNode = false; 


	u256 godMinerStart = 0;
	u256 godMinerEnd = 0;
	std::map<std::string, NodeConnParams> godMinerList;
	//for ssl 
	std::map<std::string, NodeParams> godMinerListSSL;
};

}
}
