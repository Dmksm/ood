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

	CCappuccino(CappuccinoSize size = CappuccinoSize::Standart)
		: CCoffee((size == CappuccinoSize::Standart) ? "Cappuccino" : "Double cappuccino")
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

	CLatte(LatteSize size = LatteSize::Standart)
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

	CTea(TeaVariety variety = TeaVariety::PalacePuer)
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

	CMilkshake(MilkshakeSize size = MilkshakeSize::Medium)
		: CBeverage(GetMilkshakeDescription(size))
		, m_size(size)
	{
	}

	double GetCost() const override
	{
		return GetMilkshakeCost(m_size);
	}

private:
	MilkshakeSize m_size;

	std::string GetMilkshakeDescription(MilkshakeSize size) const
	{
		switch (size)
		{
			case MilkshakeSize::Small:
			{
				return "Small milkshake";
			}
			case MilkshakeSize::Medium:
			{
				return "Medium milkshake";
			}
			case MilkshakeSize::Large:
			{
				return "Large milkshake";
			}
		}
	}

	double GetMilkshakeCost(MilkshakeSize size) const
	{
		switch (size)
		{
			case MilkshakeSize::Small:
			{
				return 50;
			}
			case MilkshakeSize::Medium:
			{
				return 60;
			}
			case MilkshakeSize::Large:
			{
				return 80;
			}
		}
	}
};