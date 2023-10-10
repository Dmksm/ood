#pragma once

#include "IBeverage.h"

// Базовая реализация напитка, предоставляющая его описание
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

// Кофе
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

// Капуччино
class CCappuccino : public CCoffee
{
public:
	CCappuccino()
		:CCoffee("Cappuccino")
	{}

	double GetCost() const override
	{
		return 80;
	}
};

class CDoubleCappuccino : public CCoffee
{
public:
	CDoubleCappuccino()
		:CCoffee("Double cappuccino")
	{}

	double GetCost() const override
	{
		return 120;
	}
};

// Латте
class CLatte : public CCoffee
{
public:
	CLatte()
		:CCoffee("Latte")
	{}

	double GetCost() const override
	{
		return 90;
	}
};

class CDoubleLatte : public CCoffee
{
public:
	CDoubleLatte()
		:CCoffee("Double latte")
	{}

	double GetCost() const override
	{
		return 130;
	}
};

// Чай
class CTea : public CBeverage
{
public:
	CTea(const std::string& description = "Tea")
		:CBeverage(description)
	{}

	double GetCost() const override
	{
		return 30;
	}
};

class СPalacePuer : public CTea
{
public:
	СPalacePuer()
		:CTea("Palace puer")
	{}
};

class СCeylonMaharaja : public CTea
{
public:
	СCeylonMaharaja()
		:CTea("Ceylon maharaja")
	{}
};

class CBaiMaoHou : public CTea
{
public:
	CBaiMaoHou()
		:CTea("Bai Mao Hou")
	{}
};

class CChocolateTruffle : public CTea
{
public:
	CChocolateTruffle()
		:CTea("Chocolate truffle")
	{}
};

// Молочный коктейль
class CMilkshake : public CBeverage
{
public:
	CMilkshake(const std::string& description = "Milkshake")
		:CBeverage("Milkshake")
	{}

	double GetCost() const override
	{
		return 80;
	}
};

class CSmallMilkshake : public CMilkshake
{
public:
	CSmallMilkshake()
		:CMilkshake("Small milkshake")
	{}

	double GetCost() const override
	{
		return 50;
	}
};

class CMediumMilkshake : public CMilkshake
{
public:
	CMediumMilkshake()
		:CMilkshake("Medium milkshake")
	{}

	double GetCost() const override
	{
		return 60;
	}
};

class CLargeMilkshake : public CMilkshake
{
public:
	CLargeMilkshake()
		:CMilkshake("Large milkshake")
	{}

	double GetCost() const override
	{
		return 80;
	}
};