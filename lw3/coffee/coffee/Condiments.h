#pragma once

#include "IBeverage.h"
#include <stdexcept>


// ������� ��������� "������� � �������". ����� �������� ��������
class CCondimentDecorator : public IBeverage
{
public:
	std::string GetDescription()const override
	{
		// �������� ��������������� ������� ����������� � �������� �������������� �������
		return m_beverage->GetDescription() + ", " + GetCondimentDescription();
	}

	double GetCost()const override
	{
		// ��������� ������������ �� ��������� ������� � ��������� ������������� �������
		return m_beverage->GetCost() + GetCondimentCost();
	}

	// ��������� � �������� ������� ����������� � ������� ���������� �����������
	virtual std::string GetCondimentDescription()const = 0;
	virtual double GetCondimentCost()const = 0;
protected:
	CCondimentDecorator(IBeveragePtr&& beverage)
		: m_beverage(move(beverage))
	{
	}
private:
	IBeveragePtr m_beverage;
};

// ������� �� ������
class CCinnamon : public CCondimentDecorator
{
public:
	CCinnamon(IBeveragePtr&& beverage)
		: CCondimentDecorator(move(beverage))
	{}
protected:
	double GetCondimentCost()const override
	{
		return 20;
	}

	std::string GetCondimentDescription()const override
	{
		return "Cinnamon";
	}
};

class CCream : public CCondimentDecorator
{
public:
	CCream(IBeveragePtr&& beverage)
		: CCondimentDecorator(std::move(beverage))
	{
	}
protected:
	double GetCondimentCost()const override
	{
		return 25;
	}

	std::string GetCondimentDescription()const override
	{
		return "Cream";
	}
};

class CChocolate : public CCondimentDecorator
{
public:
	CChocolate(IBeveragePtr&& beverage, unsigned quantity = 1)
		: CCondimentDecorator(std::move(beverage))
	{
		if (quantity < 1 || quantity > 5)
		{
			throw std::logic_error("The number of chocolate slices should be from 1 to 5!");
		}
		m_quantity = quantity;
	}
protected:
	double GetCondimentCost()const override
	{
		return 10 * m_quantity;
	}

	std::string GetCondimentDescription()const override
	{
		return "Chocolate x " + std::to_string(m_quantity);
	}
private:
	unsigned m_quantity;
};

class CLiquor : public CCondimentDecorator
{
public:
	enum class Type
	{
		Chocolate,
		Nut
	};

	CLiquor(IBeveragePtr&& beverage, Type type = Type::Nut)
		: CCondimentDecorator(std::move(beverage))
		, m_type(type)
	{
	}

protected:
	double GetCondimentCost()const override
	{
		return 50;
	}
	std::string GetCondimentDescription()const override
	{
		return std::string(m_type == Type::Nut ? "Nut" : "Chocolate") + " liquor";
	}
private:
	Type m_type;
};

// �������� �������
class CLemon : public CCondimentDecorator
{
public:
	CLemon(IBeveragePtr&& beverage, unsigned quantity = 1)
		: CCondimentDecorator(std::move(beverage))
		, m_quantity(quantity)
	{}
protected:
	double GetCondimentCost()const override
	{
		return 10.0 * m_quantity;
	}
	std::string GetCondimentDescription()const override
	{
		return "Lemon x " + std::to_string(m_quantity);
	}
private:
	unsigned m_quantity;
};

enum class IceCubeType
{
	Dry,	// ����� ��� (��� ������� ��������� �������)
	Water	// ������� ������ �� ����
};

// ������� "������ ����". ������������ ����� � ����������� �������, ��� ������ �� ���������
// � ��������
class CIceCubes : public CCondimentDecorator
{
public:
	CIceCubes(IBeveragePtr&& beverage, unsigned quantity, IceCubeType type = IceCubeType::Water)
		: CCondimentDecorator(std::move(beverage))
		, m_quantity(quantity)
		, m_type(type)
	{}
protected:
	double GetCondimentCost()const override
	{
		// ��� ������ �������, ��� ������ ���������.
		// ����� ��� ����� ������
		return (m_type == IceCubeType::Dry ? 10 : 5) * m_quantity;
	}
	std::string GetCondimentDescription()const override
	{
		return std::string(m_type == IceCubeType::Dry ? "Dry" : "Water")
			+ " ice cubes x " + std::to_string(m_quantity);
	}
private:
	unsigned m_quantity;
	IceCubeType m_type;
};

// ��� ������
enum class SyrupType
{
	Chocolate,	// ����������
	Maple,		// ��������
};

// ������� "�����"
class CSyrup : public CCondimentDecorator
{
public:
	CSyrup(IBeveragePtr&& beverage, SyrupType syrupType)
		: CCondimentDecorator(std::move(beverage))
		, m_syrupType(syrupType)
	{}
protected:
	double GetCondimentCost()const override
	{
		return 15;
	}
	std::string GetCondimentDescription()const override
	{
		return std::string(m_syrupType == SyrupType::Chocolate ? "Chocolate" : "Maple")
			+ " syrup";
	}
private:
	SyrupType m_syrupType;
};

// ���������� ������
class CChocolateCrumbs : public CCondimentDecorator
{
public:
	CChocolateCrumbs(IBeveragePtr&& beverage, unsigned mass)
		:CCondimentDecorator(std::move(beverage))
		, m_mass(mass)
	{
	}

	double GetCondimentCost()const override
	{
		return 2.0 * m_mass;
	}

	std::string GetCondimentDescription()const override
	{
		return "Chocolate crumbs " + std::to_string(m_mass) + "g";
	}
private:
	unsigned m_mass;
};

// ��������� �������
class CCoconutFlakes : public CCondimentDecorator
{
public:
	CCoconutFlakes(IBeveragePtr&& beverage, unsigned mass)
		: CCondimentDecorator(std::move(beverage))
		, m_mass(mass)
	{}

protected:
	double GetCondimentCost()const override
	{
		return 1.0 * m_mass;
	}
	std::string GetCondimentDescription()const override
	{
		return "Coconut flakes " + std::to_string(m_mass) + "g";
	}
private:
	unsigned m_mass;
};