#include <iostream>

#include "math.hh"

void Math::updateMath(QVector<MathItem> mathItems, TableWidget *table) {
    for (int i = 0; i<mathItems.size(); i++) {
        QString equ = mathItems.at(i).equation;
        QString newEqu = equ.remove(0,1);

        if ((newEqu.startsWith("add"))||(newEqu.startsWith("sub"))
                ||(newEqu.startsWith("mp"))||(newEqu.startsWith("div"))) {
            applyBulkFormula(mathItems.at(i),table);
        } else {
            applyColumnFormula(mathItems.at(i),table);
        }
    }
}

void Math::applyBulkFormula(MathItem mathItem, TableWidget *table) {
    int x = mathItem.x;
    int y = mathItem.y;
    QString equ = mathItem.equation;
    QString newEqu = equ.remove(0,1);

    bool parseOp = true;
    QString opStr = "";
    OPERATION op = OPERATION::ADD;
    bool isSecond = false;
    bool parse = false;
    bool foundComma = false;
    QString strCx = "";
    QString strCy = "";
    int cx = 0;
    int cy = 0;
    Cell startCell, endCell;
    double result = 0;

    std::cout << "Bulk Equation: " << newEqu.toStdString() << std::endl;

    for (int i = 0; i<newEqu.size(); i++) {
        QChar c = newEqu.at(i);
        if (c=='>') {
            parseOp = false;
            if (opStr=="add") {
                op = OPERATION::ADD;
            } else if (opStr=="sub") {
                op = OPERATION::SUB;
            } else if (opStr=="mp") {
                op = OPERATION::MP;
            } else if (opStr=="div") {
                op = OPERATION::DIV;
            }
        } else if (c=='(') {
            parse = true;
        } else if (c==')') {
            cx = QVariant(strCx).toInt();
            cy = QVariant(strCy).toInt();
            cx--;
            cy--;

            if (isSecond) {
                endCell.x = cx;
                endCell.y = cy;
            } else {
                startCell.x = cx;
                startCell.y = cy;
            }

            parse = false;
            foundComma = false;
            strCx = strCy = "";
        } else if (c==':') {
            isSecond = true;
        } else if (c==',') {
            foundComma = true;
        } else {
            if (parseOp) {
                opStr+=c;
            } else {
                if (parse) {
                    if (foundComma) {
                        strCx+=c;
                    } else {
                        strCy+=c;
                    }
                }
            }
        }
    }

    std::cout << "Operation: " << opStr.toStdString() << std::endl;
    std::cout << "StartCell x: " << startCell.x << std::endl;
    std::cout << "StartCell y: " << startCell.y << std::endl;
    std::cout << "EndCell x: " << endCell.x << std::endl;
    std::cout << "EndCell y: " << endCell.y << std::endl;

    bool getRows = true;
    int start = startCell.y;
    int end = endCell.y;
    if ((startCell.y)==(endCell.y)) {
        getRows = false;
        start = startCell.x;
        end = endCell.x;
    }

    std::cout << "start: " << start << std::endl;
    std::cout << "end: " << end << std::endl;

    QStringList noStrList;

    for (int i = start; i<(end+1); i++) {
        QTableWidgetItem *item;
        if (getRows) {
            item = table->item(i,startCell.x);
        } else {
            item = table->item(startCell.y,i);
        }
        if (item==nullptr) {
            continue;
        }
        noStrList.push_back(item->text());
    }

    if (noStrList.size()==0) {
        return;
    }

    int s = 0;
    if ((op==OPERATION::MP)||(op==OPERATION::DIV)) {
        result = QVariant(noStrList.at(0)).toDouble();
        s = 1;
    }
    for (int i = s; i<noStrList.size(); i++) {
        double current = QVariant(noStrList.at(i)).toDouble();

        if (op==OPERATION::ADD) {
            result+=current;
        } else if (op==OPERATION::SUB) {
            result-=current;
        } else if (op==OPERATION::MP) {
            result*=current;
        } else if (op==OPERATION::DIV) {
            result/=current;
        }
    }

    QString strResult = QVariant(result).toString();
    QTableWidgetItem *item = table->item(x,y);
    if (item==nullptr) {
        item = new QTableWidgetItem;
    }
    item->setText(strResult);
    table->setItem(x,y,item);
}

void Math::applyColumnFormula(MathItem mathItem, TableWidget *table) {
    int x = mathItem.x;
    int y = mathItem.y;
    QString equ = mathItem.equation;

    QString newEqu = equ.remove(0,1);
    bool foundSign = false;
    bool parse = false;
    bool foundComma = false;
    bool inBracket = false;
    double result = 0;
    OPERATION op = OPERATION::ADD;
    QString cxStr = "";
    QString cyStr = "";
    int cx = 0;
    int cy = 0;
    double no = 0;

    std::cout << newEqu.toStdString() << std::endl;

    for (int i = 0; i<newEqu.size(); i++) {
        QChar c = newEqu.at(i);
        if (c=='+') {
            foundSign = true;
            op = OPERATION::ADD;
        } else if (c=='-') {
            foundSign = true;
            op = OPERATION::SUB;
        } else if (c=='x') {
            foundSign = true;
            op = OPERATION::MP;
        } else if (c=='/') {
            foundSign = true;
            op = OPERATION::DIV;
        } else if (c=='[') {
            inBracket = true;
            parse = true;
        } else if (c=='(') {
            parse = true;
        } else if ((c==')')||(c==']')) {
            cx = QVariant(cxStr).toInt();
            cy = QVariant(cyStr).toInt();
            cx--;
            cy--;

            QTableWidgetItem *item = table->item(cx,cy);
            if (item!=nullptr) {
                no = QVariant(item->text()).toDouble();
            } else {
                no = QVariant(cxStr).toDouble();
            }

            if (foundSign) {
                if (op==OPERATION::ADD) {
                    result+=no;
                } else if (op==OPERATION::SUB) {
                    result-=no;
                } else if (op==OPERATION::MP) {
                    result*=no;
                } else if (op==OPERATION::DIV) {
                    result/=no;
                }
            } else {
                result = QVariant(no).toDouble();
            }

            cx = cy = no = 0;
            cxStr = "";
            cyStr = "";

            parse = false;
            foundComma = false;
            inBracket = false;
        } else if (c==',') {
            foundComma = true;
        } else {
            if (parse==false) {
                continue;
            }
            if (inBracket) {
                cxStr+=c;
            } else if (foundComma) {
                cyStr+=c;
            } else {
                cxStr+=c;
            }
        }
    }

    QString strResult = QVariant(result).toString();
    QTableWidgetItem *item = table->item(x,y);
    if (item==nullptr) {
        item = new QTableWidgetItem;
    }
    item->setText(strResult);
    table->setItem(x,y,item);
}
