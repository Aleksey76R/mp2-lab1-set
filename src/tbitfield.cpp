// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    if (len < 0)
        throw std::invalid_argument("TBitField: length must be non-negative");

    BitLen = len;
    MemLen = (len + (8 * sizeof(TELEM) - 1)) / (8 * sizeof(TELEM));

    if (len > 0){
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++){
            pMem[i] = 0;
        }
    }
    else{
        pMem = nullptr;
    }

}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;

    if (MemLen > 0)
    {
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++)
            pMem[i] = bf.pMem[i];
    }
    else
    {
        pMem = nullptr;
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
    pMem = nullptr;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n / (8 * sizeof(TELEM));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return static_cast<TELEM>(1u << (n % (8 * sizeof(TELEM))));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("TBitField::SetBit: index out of range");

    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("TBitField::ClrBit: index out of range");

    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("TBitField::GetBit: index out of range");

    return (pMem[GetMemIndex(n)] & GetMemMask(n)) ? 1 : 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this == &bf)
        return *this;

    if (MemLen != bf.MemLen)
    {
        delete[] pMem;
        MemLen = bf.MemLen;
        pMem = (MemLen > 0) ? new TELEM[MemLen] : nullptr;
    }

    BitLen = bf.BitLen;

    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];

    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{   
    if (BitLen != bf.BitLen)
        return 0;

    for (int i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i])
            return 0;

    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{    
    const TBitField &bigger  = (BitLen >= bf.BitLen) ? *this : bf;
    const TBitField &smaller = (BitLen >= bf.BitLen) ? bf    : *this;

    TBitField res(bigger.BitLen);

    for (int i = 0; i < bigger.MemLen; i++)
        res.pMem[i] = bigger.pMem[i];

    for (int i = 0; i < smaller.MemLen; i++)
        res.pMem[i] |= smaller.pMem[i];

    return res;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    const TBitField &bigger  = (BitLen >= bf.BitLen) ? *this : bf;
    const TBitField &smaller = (BitLen >= bf.BitLen) ? bf    : *this;

    TBitField res(bigger.BitLen);

    for (int i = 0; i < smaller.MemLen; i++)
        res.pMem[i] = bigger.pMem[i] & smaller.pMem[i];

    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(BitLen);

    for (int i = 0; i < MemLen; i++)
        res.pMem[i] = ~pMem[i];

    int tail = BitLen % (8 * sizeof(TELEM));
    if (tail != 0 && MemLen > 0)
    {
        TELEM mask = static_cast<TELEM>((1u << tail) - 1u);

        res.pMem[MemLen - 1] &= mask;
    }

    return res;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    for (int i = 0; i < bf.BitLen; i++)
    {
        int bit;
        istr >> bit;
        if (bit)
            bf.SetBit(i);
        else
            bf.ClrBit(i);
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = bf.BitLen - 1; i >= 0; i--)
        ostr << bf.GetBit(i);
    return ostr;
}
