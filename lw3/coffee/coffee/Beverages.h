#pragma once

#include "IBeverage.h"
#include <map>

class CBeverage : public IBeverage
{
public:
	CBeverage(const std::string& description)
		:m_description(description)
	{}

	std::string GetDescription()const override final
	{
		return m_description;
	}
private:
	std::string m_description;
};

class CCoffee : public CBeverage
{
public:
	CCoffee(const std::string& description = "Coffee")
		:CBeverage(description)
	{}

	double GetCost() const override
	{
		return 60;
	}
};

class CCappuccino : public CCoffee
{
public:
	enum class CappuccinoSize
	{
		Standart,
		Double
	};

	CCappuccino(CappuccinoSize size)
		:CCoffee((size == CappuccinoSize::Standart) ? "Cappuccino" : "Double cappuccino")
		, m_size(size)
	{
	}

	double GetCost() const override
	{
		return (m_size == CappuccinoSize::Standart) ? 80 : 120;
	}
private:
	CappuccinoSize m_size;
};

class CLatte : public CCoffee
{
public:
	enum class LatteSize
	{
		Standart,
		Double
	};

	CLatte(LatteSize size)
		: CCoffee((size == LatteSize::Standart) ? "Latte" : "Double latte")
		, m_size(size)
	{
	}

	double GetCost() const override
	{
		return (m_size == LatteSize::Standart) ? 90 : 130;
	}
private:
	LatteSize m_size;
};

class CTea : public CBeverage
{
public:
	enum class TeaVariety
	{
		PalacePuer,
		CeylonMaharaja,
		BaiMaoHou,
		ChocolateTruffle
	};

	CTea(TeaVariety variety)
		: CBeverage(GetTeaDescription(variety))
	{
	}

	double GetCost() const override
	{
		return 30;
	}

private:
	std::string GetTeaDescription(TeaVariety variety)
	{
		switch (variety)
		{
			case TeaVariety::PalacePuer:
			{
				return "PalacePuer";
			}
			case TeaVariety::BaiMaoHou:
			{
				return "BaiMaoHou";
			}
			case TeaVariety::CeylonMaharaja:
			{
				return "CeylonMaharaja";
			}
			case TeaVariety::ChocolateTruffle:
			{
				return "ChocolateTruffle";
			}
		}
	}
};

class CMilkshake : public CBeverage
{
public:
	enum class MilkshakeSize
	{
		Small,
		Medium,
		Large
	};

	CMilkshake(MilkshakeSize size)
		:CBeverage(GetMilkshakeDescription(size))
		, m_size(size)
	{
	}

	double GetCost() const override
	{
		return GetMilkshakeCost(m_size);
	}

private:
	MilkshakeSize m_size;

	std::map<MilkshakeSize, std::pair<std::string, double>> m_milkshakeInfo =
		{
			{
				MilkshakeSize::Small, {"Small milkshake", 50}
			},
			{
				MilkshakeSize::Medium, {"Medium milkshake", 60}
			},
			{
				MilkshakeSize::Large, {"Large milkshake", 80}
			},
		};

	std::string GetMilkshakeDescription(MilkshakeSize size) const
	{
		return m_milkshakeInfo.at(size).first;
	}

	double GetMilkshakeCost(MilkshakeSize size) const
	{
		return m_milkshakeInfo.at(size).second;
	}
};