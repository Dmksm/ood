#pragma once

#include "IBeverage.h"
#include <map>
#include <stdexcept>


//программа должна компилироваться без варнингов
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
	enum class Size //можно не указывать конкретно что это капучино сиче просто ссайз
	{
		Standart,
		Double
	};

	CCappuccino(Size size = Size::Standart)
		: CCoffee((size == Size::Standart) ? "Cappuccino" : "Double cappuccino")
		, m_size(size)
	{
	}

	double GetCost() const override
	{
		return (m_size == Size::Standart) ? 80 : 120;
	}
private:
	Size m_size;
};

class CLatte : public CCoffee
{
public:
	enum class Size
	{
		Standart,
		Double
	};

	CLatte(Size size = Size::Standart)
		: CCoffee((size == Size::Standart) ? "Latte" : "Double latte")
		, m_size(size)
	{
	}

	double GetCost() const override
	{
		return (m_size == Size::Standart) ? 90 : 130;
	}
private:
	Size m_size;
};

class CTea : public CBeverage
{
public:
	enum class Variety
	{
		PalacePuer,
		CeylonMaharaja,
		BaiMaoHou,
		ChocolateTruffle
	};

	CTea(Variety variety = Variety::PalacePuer)
		: CBeverage(GetTeaDescription(variety))
	{
	}

	double GetCost() const override
	{
		return 30;
	}

private:
	std::string GetTeaDescription(Variety variety)
	{
		switch (variety)
		{
			case Variety::PalacePuer:
			{
				return "PalacePuer";
			}
			case Variety::BaiMaoHou:
			{
				return "BaiMaoHou";
			}
			case Variety::CeylonMaharaja:
			{
				return "CeylonMaharaja";
			}
			case Variety::ChocolateTruffle:
			{
				return "ChocolateTruffle";
			}
			default:
			{
				throw std::logic_error("Unknown tea variety!");
			}
		}
	}
};

class CMilkshake : public CBeverage
{
public:
	enum class Size
	{
		Small,
		Medium,
		Large
	};

	CMilkshake(Size size = Size::Medium)
		: CBeverage(GetMilkshakeDescription(size))
		, m_size(size)
	{
	}

	double GetCost() const override
	{
		return GetMilkshakeCost(m_size);
	}

private:
	Size m_size;

	std::string GetMilkshakeDescription(Size size) const
	{
		switch (size)
		{
			case Size::Small:
			{
				return "Small milkshake";
			}
			case Size::Medium:
			{
				return "Medium milkshake";
			}
			case Size::Large:
			{
				return "Large milkshake";
			}
			default:
			{
				throw std::logic_error("Unknown milkshake size!");
			}
		}
	}

	double GetMilkshakeCost(Size size) const
	{
		switch (size)
		{
			case Size::Small:
			{
				return 50;
			}
			case Size::Medium:
			{
				return 60;
			}
			case Size::Large:
			{
				return 80;
			}
			default:
			{
				throw std::logic_error("Unknown milkshake size!");
			}
		}
	}
};