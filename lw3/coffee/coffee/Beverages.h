#pragma once

#include "IBeverage.h"

// ������� ���������� �������, ��������������� ��� ��������
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

// ����
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

// ���������
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

// �����
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

// ���
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

class �PalacePuer : public CTea
{
public:
	�PalacePuer()
		:CTea("Palace puer")
	{}
};

class �CeylonMaharaja : public CTea
{
public:
	�CeylonMaharaja()
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

// �������� ��������
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