// This file is part of CppSheets.
//
// Copyright 2018 Patrick Flynn <patrick_dev2000@outlook.com>
//
// CppSheets is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// CppSheets is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with CppSheets. If not, see <https://www.gnu.org/licenses/>.
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <iostream>

#include "tablewidget.hh"
#include "tablewidget_delegate.hh"
#include "formula/math.hh"

TableWidget::TableWidget() {
    this->setColumnCount(30);
    this->setRowCount(30);
    this->loadHeaders();

    this->setItemDelegate(new TableWidgetDelegate(this));

    connect(this,&TableWidget::cellChanged,this,&TableWidget::onCellChanged);
    connect(this,SIGNAL(itemChanged(QTableWidgetItem*)),this,SLOT(onItemChanged(QTableWidgetItem*)));
    connect(this,SIGNAL(cellClicked(int,int)),this,SLOT(onCellClicked(int,int)));
    connect(this,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(onCurrentCellChanged(int,int,int,int)));
}

void TableWidget::loadHeaders() {
    QStringList headers;
    QStringList letters;

    QString first = "";
    QString current = "";

    for (char c = 'A'; c <= 'Z'; c++) {
        current += c;
        letters.push_back(current);
        current = "";
    }

    int len = letters.length();
    int index = 0;
    int fIndex = 0;

    for (int i = 0; i<1000; i++) {
        if (index == len) {
            index = 0;

            first = letters.at(fIndex);
            fIndex++;

            if (fIndex == len) {
                fIndex = 0;
            }
        }
\
        current = first;
        current += letters.at(index);
        headers.push_back(current);
        current = "";

        index++;
    }

    this->setHorizontalHeaderLabels(headers);
}

void TableWidget::setMathItems(QVector<MathItem> items) {
    mathItems = items;
}

void TableWidget::addMathItem(MathItem item) {
    for (int i = 0; i<mathItems.size(); i++) {
        int x = mathItems.at(i).x;
        int y = mathItems.at(i).y;

        if ((x==item.x)&&(y==item.y)) {
            mathItems.removeAt(i);
            break;
        }
    }
    if (item.equation!="=\\") {
        mathItems.push_back(item);
    }
}

QVector<MathItem> TableWidget::allMathItems() {
    return mathItems;
}

void TableWidget::updateMath() {
    Math::updateMath(mathItems,this);
}

bool TableWidget::isMath(int row, int col) {
    bool ret = false;

    for (int i = 0; i<mathItems.size(); i++) {
        int x = mathItems.at(i).x;
        int y = mathItems.at(i).y;

        if ((x==row)&&(y==col)) {
            ret = true;
            break;
        }
    }

    return ret;
}

QString TableWidget::formula(int row, int col) {
    QString str = "";

    for (int i = 0; i<mathItems.size(); i++) {
        int x = mathItems.at(i).x;
        int y = mathItems.at(i).y;

        if ((x==row)&&(y==col)) {
            str = mathItems.at(i).equation;
            break;
        }
    }
    return str;
}

QModelIndexList TableWidget::currentSelectedItems() {
    return this->selectedIndexes();
}

void TableWidget::addGraphItem(GraphItem item) {
    for (int i = 0; i<graphItems.size(); i++) {
        if (graphItems.at(i).name == item.name) {
            graphItems.remove(i);
        }
    }

    graphItems << item;
}

QVector<GraphItem> TableWidget::allGraphItems() {
    return graphItems;
}

void TableWidget::keyPressEvent(QKeyEvent *event) {
    //Logic for the enter or return keys
    if ((event->key()==Qt::Key_Enter)||(event->key()==Qt::Key_Return)) {
        QTableWidget::keyPressEvent(event);
        int x = this->currentRow()+1;
        int y = this->currentColumn();
        this->setCurrentCell(x,y);

    //Logic for the delete key
    //Clear all cells when pressed
    } else if (event->key()==Qt::Key_Delete) {
        auto list = currentSelectedItems();

        //Clear one cell
        if (list.size()==1) {
            QTableWidgetItem *item = currentItem();
            item->setText("");
            this->setItem(currentRow(),currentColumn(),item);

        //Clear multiple cells
        } else {
            for (auto current : list) {
                QTableWidgetItem *item = this->item(current.row(),current.column());
                if (item!=nullptr) {
                    item->setText("");
                }
                this->setItem(current.row(),current.column(),item);
            }
        }
    }

    QTableWidget::keyPressEvent(event);
}

void TableWidget::onCellChanged() {
    emit cellModified();
}

void TableWidget::onCellClicked(int row, int col) {
}

void TableWidget::onItemChanged(QTableWidgetItem *item) {
    if (item->text().startsWith("=")) {
        MathItem mitem;
        mitem.x = item->row();
        mitem.y = item->column();
        mitem.equation = item->text();
        addMathItem(mitem);
    }
    updateMath();
}

void TableWidget::onCurrentCellChanged(int currentRow, int currentCol, int oldRow, int oldCol) {
    if (currentRow == this->rowCount()-1) {
        for (int i = 0; i<10; i++) {
            this->insertRow(currentRow+i);
        }
    }
    if (currentCol == this->columnCount()-1) {
        for (int i = 0; i<10; i++) {
            this->insertColumn(currentCol+i);
        }
    }

    this->setCurrentCell(currentRow,currentCol);
    loadHeaders();
}
