// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"
#include <stdexcept>

TSet::TSet(int mp) : BitField(mp)
{
    MaxPower = mp;
}

TSet::TSet(const TSet &s) : BitField(s.BitField)
{
    MaxPower = s.MaxPower;
}

TSet::TSet(const TBitField &bf) : BitField(bf)
{
    MaxPower = bf.GetLength();
}

TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower(void) const
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const
{
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem)
{
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem)
{
    BitField.ClrBit(Elem);
}

TSet& TSet::operator=(const TSet &s)
{
    if (this == &s)
        return *this;
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

int TSet::operator==(const TSet &s) const
{
    if (MaxPower != s.MaxPower)
        return 0;
    return BitField == s.BitField;
}

int TSet::operator!=(const TSet &s) const
{
    return !(*this == s);
}

TSet TSet::operator+(const TSet &s)
{
    TBitField resField = BitField | s.BitField;
    return TSet(resField);
}

TSet TSet::operator+(const int Elem)
{
    if (Elem < 0 || Elem >= MaxPower)
        throw std::out_of_range("TSet::operator+: element out of range");

    TSet res(*this);
    res.InsElem(Elem);
    return res;
}

TSet TSet::operator-(const int Elem)
{
    if (Elem < 0 || Elem >= MaxPower)
        throw std::out_of_range("TSet::operator-: element out of range");

    TSet res(*this);
    res.DelElem(Elem);
    return res;
}

TSet TSet::operator*(const TSet &s)
{
    TBitField resField = BitField & s.BitField;
    return TSet(resField);
}

TSet TSet::operator~(void)
{
    TBitField resField = ~BitField;
    return TSet(resField);
}

istream &operator>>(istream &istr, TSet &s)
{
    int val;
    while (istr >> val)
    {
        if (val >= 0 && val < s.MaxPower)
            s.InsElem(val);
    }
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s)
{
    ostr << "{ ";
    bool first = true;
    for (int i = 0; i < s.MaxPower; i++)
    {
        if (s.IsMember(i))
        {
            if (!first)
                ostr << ", ";
            ostr << i;
            first = false;
        }
    }
    ostr << " }";
    return ostr;
}