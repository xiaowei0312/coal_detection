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
    double dTmp = 0;    //����
    double tmp = 10;       //С���������ӣ�10,100-->1000
    
    while(1)
    {
        char c = *szFormula++;
        if(c>='0' && c<='9')
        {
            if(status == 0)
            {
                dTmp = 0;
                tmp = 10;
                status = 1; //��������
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
            status = 2; //��������(С����֮��)
        else
        {
            if(status != 0) //���ִ������
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
            
            if(c == 'n')    //��ʽ�е������ַ���ni ���������б��е��±�i+1  i<10
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
