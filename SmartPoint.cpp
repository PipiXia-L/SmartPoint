//前两个例子只能用于管理Point类的基础对象
//本例中我们设计模板　使我们的智能指针适用于更多的基础对象类

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <set>
#include <cstring>
#include <cmath>
#include <cstdio>
#include <stack>
#include <queue>
using namespace std;

//基础对象类
class Point
{
public:
    Point(int xVal = 0, int yVal = 0) :x(xVal),y(yVal) {  }
    int getX() const { return x; }
    int getY() const { return y; }
    void setX(int xVal) { x = xVal; }
    void setY(int yVal) { y = yVal; }
private:
    int x;
    int y;
};

/*
 * 辅助类，成员全部是私有，只为智能指针使用
 * 用以封装使用计数与基础对象指针
*/
//模板类作为友元时要先有声明
template <typename T>
class SmartPtr;

template <typename T>
class U_ptr
{
private:
    //该类成员访问权限全部为private,因为不想让用户直接使用该类
    friend class SmartPtr<T>; //因为智能指针需要直接操纵辅助类

    //构造函数的参数为基础对象的指针
    U_ptr(T *ptr): p(ptr),count(1) {}

    ~U_ptr() { delete p; }

    //引用计数
    int count;

    //基础对象指针
    T *p;
};

template <typename T>
class SmartPtr   //智能指针类
{
public:
    SmartPtr(T *ptr) :rp(new U_ptr<T>(ptr)) {} //构造函数

    /*复制构造函数*/
    SmartPtr(const SmartPtr &sp) :rp(sp.rp) { ++rp->count; }

    //重载赋值操作符
    SmartPtr& operator=(const SmartPtr<T>& rhs)
    {
        ++rhs.rp->count;  //首先将右操作数计数加１
        if(--rp->count == 0)
            delete rp;
        rp = rhs.rp;
        return *this;
    }

    ~SmartPtr()
    {
        if(--rp->count == 0)
        //当引用计数减为0,释放指针，删除对象
            delete rp;
        else
            cout << "还有" << rp->count << "　个指针指向基础对象" << endl;
    }

    T & operator *()  //重载*操作符
    {
        return *(rp->p);
    }
    T * operator ->()  //重载->操作符
    {
        return rp->p;
    }
private:
    U_ptr<T> *rp;  //辅助类对象指针
};

int main()
{
    int *i = new int(2);
    {
        SmartPtr<int> ptr1(i);
        {
            SmartPtr<int> ptr2(ptr1);
            {
                SmartPtr<int> ptr3 = ptr1;

                cout << *ptr1 << endl;

                *ptr1 = 20;
                cout << *ptr2 << endl;
            }
        }
    }

    return 0;
}
