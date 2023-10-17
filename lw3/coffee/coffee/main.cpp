#include "Beverages.h"
#include "Condiments.h"

#include <iostream>
#include <string>
#include <functional>

using namespace std;


/*
�������������� ������, ��������� �������� �������
*/
struct MakeLemon
{
	MakeLemon(unsigned quantity)
		:m_quantity(quantity)
	{}

	auto operator()(IBeveragePtr&& beverage)const
	{
		return make_unique<CLemon>(move(beverage), m_quantity);
	}
private:
	unsigned m_quantity;
};

/*
�������, ������������ �������, ��������� �������� �������
*/
function<IBeveragePtr(IBeveragePtr&&)> MakeCinnamon()
{
	return [](IBeveragePtr&& b) {
		return make_unique<CCinnamon>(move(b));
		};
}

/*
���������� �������, ������������ ������� ������������ ��������

��������� �������:
	Condiment - ����� �������, ����������� �������� � �������� ������� ���������
				��������� IBeveragePtr&& �������������� �������
	Args - ������ ����� ������ ���������� ������������ (��������, ������)
*/
template <typename Condiment, typename... Args>
auto MakeCondiment(const Args&...args)
{
	// ���������� �������, ������������ �������, ���������� �� � �������� ���������
	// �������������� ��������� ����������, ����������� ������-��������, ����������
	// ������������ ���������� ����� make_unique
	return [=](auto&& b) {
		// ������� make_unique �������� b ������ �� ������� ���������� ������� �������
		return make_unique<Condiment>(forward<decltype(b)>(b), args...);
		};
}

/*
������������� ������ ��������� <<, ������� ������������� ��� �������������� �����
��� ������������� ����������

��������� ������� ������� ������������� ������� ����������� ��������� �������:
auto beverage = make_unique<CConcreteBeverage>(a, b, c)
					<< MakeCondimentA(d, e, f)
					<< MakeCondimentB(g, h);

�������������� ������� MakeCondiment* ���������� ���������, ����������� ��� ��������
����������, � ���������� ��������� �������, ����������� ������������� �������, �������
��� ����� ������ ������� ������ ������ Condiment, ������� ��� ����������� ���������.
�������������:
	auto beverage =
		make_unique<CConcreteBeverage>(a, b, c)
		<< MakeCondimentA(d, e, f)
		<< MakeCondimentB(g, h);
��� ���� ���:
	auto beverage =
		make_unique<CConcreteBeverage>
		<< MakeCondiment<CondimentA>(d, e, f)
		<< MakeCondiment<CondimentB>(g, h);
� ��������� ������ ��� ������������� ������ ������� ���������� MakeCondimentA � MakeCondimentB, �.�.
����������� ���������� ����������� ����������

������������ ������ ������������ �������� �� ���:
	auto baseBeverage = make_unique<CConcretedBeverage>(a, b, c);
	auto wrappedWithCondimentA = make_unique<CCondimentA>(move(baseBeverage), d, e, f);
	auto beverage = make_unique<CCondimentB>(move(wrappedWithCondimentA), g, h);
���� ���:
	auto beverage = make_unique<CCondimentB>(
						make_unique<CCondimentA>(
							make_unique<CConcreteBeverage>(a, b, c), // �������
							d, e, f	// ���. �������� CondimentA
						),
						g, h		// ���. ��������� CondimentB
					);

unique_ptr<CLemon> operator << (IBeveragePtr && lhs, const MakeLemon & factory)
{
	return factory(move(lhs));
}
unique_ptr<CCinnamon> operator << (IBeveragePtr && lhs, const MakeCinnamon & factory)
{
	return factory(move(lhs));
}
*/
template <typename Component, typename Decorator>
auto operator << (Component&& component, const Decorator& decorate)
{
	return decorate(forward<Component>(component));
}

void DialogWithUser()
{
	cout << "Type 1 for coffee or 2 for tea 3 for Milkshake" << std::endl;
	int beverageChoice;
	cin >> beverageChoice;

	unique_ptr<IBeverage> beverage;

	if (beverageChoice == 1)
	{
		beverage = make_unique<CCoffee>();

		int coffeeChoice;
		cout << "1 - Cappuccino, 2 - Latte" << endl;
		cin >> coffeeChoice;

		if (coffeeChoice == 1)
		{
			int portionChoice;
			cout << "1 - Standard, 2 - Double" << endl;
			cin >> portionChoice;

			if (portionChoice == 2)
			{
				beverage = make_unique<CCappuccino>(CCappuccino::Size::Double);
			}
			else if (portionChoice == 1)
			{
				beverage = make_unique<CCappuccino>();
			}
			else
			{
				return;
			}
		}
		else if (coffeeChoice == 2)
		{

			int portionChoice;
			cout << "1 - Standard, 2 - Double" << endl;
			cin >> portionChoice;

			if (portionChoice == 2)
			{
				beverage = make_unique<CLatte>(CLatte::Size::Double);
			}
			else if (portionChoice == 1)
			{
				beverage = make_unique<CLatte>();
			}
			else
			{
				return;
			}
		}
		else
		{
			return;
		}
	}
	else if (beverageChoice == 2)
	{
		int teaChoice;
		cout << "1 - Bai Mao Hou, 2 - Ceylon Maharaja, 3 - Chocolate Truffle, 4 - Palace Puer" << endl;
		cin >> teaChoice;
		if (teaChoice == 1)
		{
			beverage = make_unique<CTea>(CTea::Variety::BaiMaoHou);
		}
		else if (teaChoice == 2)
		{
			beverage = make_unique<CTea>(CTea::Variety::CeylonMaharaja);
		}
		else if (teaChoice == 3)
		{
			beverage = make_unique<CTea>(CTea::Variety::ChocolateTruffle);
		}
		else if (teaChoice == 4)
		{
			beverage = make_unique<CTea>(CTea::Variety::PalacePuer);
		}
		else
		{
			return;
		}

	}
	else if (beverageChoice == 3)
	{
		int sizeChoice;
		cout << "1 - Small, 2 - Medium, 3 - Large" << endl;
		cin >> sizeChoice;

		if (sizeChoice == 1)
		{
			beverage = make_unique<CMilkshake>(CMilkshake::Size::Small);
		}
		else if (sizeChoice == 2)
		{
			beverage = make_unique<CMilkshake>(CMilkshake::Size::Medium);
		}
		else if (sizeChoice == 3)
		{
			beverage = make_unique<CMilkshake>(CMilkshake::Size::Large);
		}
		else
		{
			return;
		}
	}
	else
	{
		return;
	}

	int condimentChoice;
	for (;;)
	{
		cout << "1 - Lemon, 2 - Cinnamon, 3 - Cream, 4 - Chocolate, 0 - Checkout" << endl;
		cin >> condimentChoice;

		if (condimentChoice == 1)
		{
			beverage = move(beverage) << MakeCondiment<CLemon>(1);
		}
		else if (condimentChoice == 2)
		{
			beverage = move(beverage) << MakeCondiment<CCinnamon>();
		}
		else if (condimentChoice == 3)
		{
			beverage = move(beverage) << MakeCondiment<CCream>();
		}
		else if (condimentChoice == 4)
		{
			int chocolateQuantity;
			cout << "Enter quantity of chocolate" << endl;
			cin >> chocolateQuantity;

			try
			{
				beverage = move(beverage) << MakeCondiment<CChocolate>(chocolateQuantity);
			}
			catch (std::exception& e)
			{
				std::cout << e.what() << std::endl;
			}
		}
		else if (condimentChoice == 0)
		{
			break;
		}
		else
		{
			return;
		}
	}
	cout << beverage->GetDescription() << ", cost: " << beverage->GetCost() << endl;
}


int main()
{
	DialogWithUser();
	cout << endl;
	{
		// �������� ������� �����
		auto latte = make_unique<CLatte>();
		// ��������� ������
		auto cinnamon = make_unique<CCinnamon>(move(latte));
		// ��������� ���� ����� ������
		auto lemon = make_unique<CLemon>(move(cinnamon), 2);
		// ��������� ���� ������� ����
		auto iceCubes = make_unique<CIceCubes>(move(lemon), 2, IceCubeType::Dry);
		// ��������� 2 ������ ���������� ������
		auto beverage = make_unique<CChocolateCrumbs>(move(iceCubes), 2);

		// ���������� ���� ����������
		cout << beverage->GetDescription() << " costs " << beverage->GetCost() << endl;
	}

	{
		auto beverage =
			make_unique<CChocolateCrumbs>(			// ������� ����: ���������� ������
				make_unique<CIceCubes>(				// | ��� ��� - ������ ����
					make_unique<CLemon>(			// | | ��� ���� �����
						make_unique<CCinnamon>(		// | | | ����� ���� - ������
							make_unique<CLatte>()),	// | | |   � ����� ������ - �����
						2),							// | | 2 ������ ������
					2, IceCubeType::Dry),			// | 2 ������ ������ ����
				2);									// 2 ������ ���������� ������

		// ���������� ���� ����������
		cout << beverage->GetDescription() << " costs " << beverage->GetCost() << endl;
	}

	// ��������� ���������� ������ MakeCondiment � ��������� <<
	{
		// lemon - �������, ����������� "2 ������ ������" � ������ �������
		auto lemon2 = MakeCondiment<CLemon>(2);
		// iceCubes - �������, ����������� "3 ������� ����" � ������ �������
		auto iceCubes3 = MakeCondiment<CIceCubes>(3, IceCubeType::Water);

		auto tea = make_unique<CTea>();

		// ���������� ��� ����� �������� ������ � ����� ��������� ����
		auto lemonIceTea = iceCubes3(lemon2(move(tea)));
		/* ���������� ������ ��������� �� �� ��������, ��� � ��������� ���:
		auto lemonIceTea =
			make_unique<CIceCubes>(
				make_unique<CLemon>(
					move(tea),
					2),
				2, IceCubeType::Water);
		*/

		auto oneMoreLemonIceTea =
			make_unique<CTea>()	// ����� ���
			<< MakeCondiment<CLemon>(2)	// ��������� ���� ����� ������
			<< MakeCondiment<CIceCubes>(3, IceCubeType::Water); // � 3 ������ ����
		/*
		���������� ����������� ������ �� �� �����, ��� � ���������:
		auto oneMoreLemonIceTea =
			MakeCondiment<CIceCubes>(3, IceCubeType::Water)(
				MakeCondiment<CLemon>(2)(make_unique<CTea>())
				);
		*/
	}

	// ������ ����������� ������� � �������� ��������������� ������
	// ��������������� ����������� << � �������� MakeCondiment
	{
		auto beverage =
			make_unique<CLatte>()							// �������� ������� �����,
			<< MakeCondiment<CCinnamon>()					// ����������� �������,
			<< MakeCondiment<CLemon>(2)						// ��������� ���� ����� ������
			<< MakeCondiment<CIceCubes>(2, IceCubeType::Dry)// ������� ���� ������� ������ ����
			<< MakeCondiment<CChocolateCrumbs>(2);			// �������� ���������� �������

		// ���������� ���� ����������
		cout << beverage->GetDescription() << " costs " << beverage->GetCost() << endl;
	}

	{
		auto beverage =
			make_unique<CMilkshake>()					// �������� �������� ��������
			<< MakeCondiment<CSyrup>(SyrupType::Maple)	// �������� �������� �������
			<< MakeCondiment<CCoconutFlakes>(8);		// �������� ��������� ��������

		// ���������� ���� ����������
		cout << beverage->GetDescription() << " costs " << beverage->GetCost() << endl;
	}
}