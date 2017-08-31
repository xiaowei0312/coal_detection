#include "data_dependency.h"
#include <QStack>
#include <QDebug>

DataDependency::DataDependency()
{
    
}

bool DataDependency::depHasEmptyElement()
{
    for(int i=0;i<depList.size();i++)
        if(depList[i]->text().trimmed().isEmpty()) 
            return true;
    return false;
}

void DataDependency::markEmptyDepElement()
{
       
}

double DataDependency::__execFormula(char *szFormula)
{
    QStack<double> stack1;
    QStack<char> stack2;
    
    int status = 0;
    double dTmp = 0;    //数字
    double tmp = 10;       //小数计算因子，10,100-->1000
    
    while(1)
    {
        char c = *szFormula++;
        if(c>='0' && c<='9')
        {
            if(status == 0)
            {
                dTmp = 0;
                tmp = 10;
                status = 1; //计算数字
            }
            if(status == 1)
                dTmp = dTmp * 10 + (c - '0');
            else if(status == 2)
            {
                dTmp = dTmp + (c-'0')/tmp;
                tmp *= 10;
            }
        }
        else if(c=='.')
            status = 2; //计算数字(小数点之后)
        else
        {
            if(status != 0) //数字处理完毕
            {
                if(!stack2.empty())
                {    
                    switch(stack2.top())
                    {
                    case '-':
                        dTmp *= -1;
                        stack2.pop();
                        stack2.push('+');
                        break;
                    case '*':
                        dTmp *= stack1.pop();
                        stack2.pop();
                        break;
                    case '/':
                        dTmp = stack1.pop() / dTmp;
                        stack2.pop();
                        break;
                    }
                }
                stack1.push(dTmp);
                status = 0;
            }
            switch(c)
            {
            case ' ':
                continue;
            case '+':
            case '-':
            case '*':
            case '/':
                stack2.push(c);
                continue;
            }
            
            if(c == 'n')    //公式中的特殊字符，ni 代表依赖列表中的下标i+1  i<10
            {
                status = 2;
                c = *szFormula++;
                dTmp = depList[c-'0'-1]->text().toDouble();
            }
            
            else if(c=='(')
            {
                status = 2;
                char *szTmp = szFormula;
                int count = 0;
                while(*szTmp && count!=1)
                {
                    char t = *szTmp++;
                    if(t == '(')
                        count--;
                    else if(t == ')')
                        count++;
                }
                *szTmp = 0;
                dTmp = __execFormula(szFormula);
                szFormula = szTmp + 1;
            }
            
            else if(c == 0)
                break;
        }
    }
    
    qDebug() << stack1;
    qDebug() << stack2;
    while(!stack2.empty())
    {
        stack1.push(stack1.pop() + stack1.pop());
        stack2.pop();
    }
    
    return stack1.pop();
}

double DataDependency::execFormula()
{
    QByteArray ba = formula.toLatin1();
    char *szFormula = ba.data();
    return __execFormula(szFormula);
}
