// Copyright 2018 Patrick Flynn
//
// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, 
//	this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this 
//	list of conditions and the following disclaimer in the documentation and/or 
//	other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may 
//	be used to endorse or promote products derived from this software 
//	without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#include <QFile>
#include <QTextStream>
#include <QVariant>
#include <iostream>

#include "parser.hh"

void Parser::createFile(QString filePath) {
    QFile file(filePath);
    if (file.open(QFile::ReadWrite | QFile::Truncate)) {
        QTextStream writer(&file);
        QString content = "<?xml version=\"1.0\"?>\n"
                          "<sheet>\n</sheet>\n";
        writer << content;
        writer.flush();
        file.close();
    }
}

QStringList Parser::pages(QString file) {
	QStringList list;
	
	XMLDocument *doc = new XMLDocument;
	doc->LoadFile(file.toStdString().c_str());

    XMLElement *root = doc->FirstChildElement("sheet");
    if (root==nullptr) {
        return list;
    }

    XMLElement *page = root->FirstChildElement("page");
    XMLElement *lastPage;

    while (page!=nullptr) {
        QString name = QString(page->Attribute("title"));
        if (!name.isNull()) {
            list.push_back(name);
        }

        lastPage = page;
        page = lastPage->NextSiblingElement("page");
    }
	
	return list;
}

bool Parser::pageExists(QString file, QString pageName) {
    bool ret = false;

    XMLDocument *doc = new XMLDocument;
    doc->LoadFile(file.toStdString().c_str());

    XMLElement *root = doc->FirstChildElement("sheet");
    if (root==nullptr) {
        return ret;
    }

    XMLElement *page = root->FirstChildElement("page");
    XMLElement *lastPage;

    while (page!=nullptr) {
        QString name = QString(page->Attribute("title"));
        if (!name.isNull()) {
            if (name==pageName) {
                ret = true;
                break;
            }
        }

        lastPage = page;
        page = lastPage->NextSiblingElement("page");
    }

    return ret;
}

QVector<SheetItem> Parser::allItems(QString file, QString page) {
    QVector<SheetItem> items;

    XMLDocument *doc = new XMLDocument;
    doc->LoadFile(file.toStdString().c_str());

    XMLElement *root = doc->FirstChildElement("sheet");
    if (root==nullptr) {
        return items;
    }

    XMLElement *pageElement = getPageElement(root,page);
    if (pageElement==nullptr) {
        return items;
    }

    XMLElement *dataElement = pageElement->FirstChildElement("data");
    if (dataElement==nullptr) {
        return items;
    }

    XMLElement *td = dataElement->FirstChildElement("td");
    XMLElement *tdOld;

    while (td!=nullptr) {
        SheetItem item;
        QString sx = QString(td->Attribute("x"));
        QString sy = QString(td->Attribute("y"));
        QString text = QString(td->GetText());

        QString bgColor = QString(td->Attribute("bgColor"));
        if (bgColor.isNull()) {
            item.bgColor = Qt::white;
        } else {
            item.bgColor = QColor(bgColor);
        }

        QString fgColor = QString(td->Attribute("fgColor"));
        if (fgColor.isNull()) {
            item.fgColor = Qt::black;
        } else {
            item.fgColor = QColor(fgColor);
        }

        QString tooltip = QString(td->Attribute("tooltip"));
        if (!tooltip.isNull()) {
            item.tooltip = tooltip;
        }

        QString colWidthStr = QString(td->Attribute("colwidth"));
        int colWidth = QVariant(colWidthStr).toInt();
        item.colWidth = colWidth;

        QString rowWidthStr = QString(td->Attribute("rowwidth"));
        int rowWidth = QVariant(rowWidthStr).toInt();
        item.rowWidth = rowWidth;

        QString sSpanX = QString(td->Attribute("spanx"));
        int spanX = QVariant(sSpanX).toInt();
        item.spanx = spanX;

        QString sSpanY = QString(td->Attribute("spany"));
        int spanY = QVariant(sSpanY).toInt();
        item.spany = spanY;

        if ((!sx.isNull())||(!sy.isNull())||(!text.isNull())) {
            item.x = QVariant(sx).toInt();
            item.y = QVariant(sy).toInt();
            item.data = text;
            items.push_back(item);
        }

        tdOld = td;
        td = tdOld->NextSiblingElement("td");
    }

    return items;
}

QVector<MathItem> Parser::allMathItems(QString file, QString page) {
    QVector<MathItem> items;

    XMLDocument *doc = new XMLDocument;
    doc->LoadFile(file.toStdString().c_str());

    XMLElement *root = doc->FirstChildElement("sheet");
    if (root==nullptr) {
        return items;
    }

    XMLElement *pageElement = getPageElement(root,page);
    if (pageElement==nullptr) {
        return items;
    }

    XMLElement *mathElement = pageElement->FirstChildElement("math");
    if (mathElement==nullptr) {
        return items;
    }

    XMLElement *td = mathElement->FirstChildElement("equ");
    XMLElement *tdOld;

    while (td!=nullptr) {
        MathItem item;
        QString sx = QString(td->Attribute("x"));
        QString sy = QString(td->Attribute("y"));
        QString equation = QString(td->GetText());

        if ((!sx.isNull())||(!sy.isNull())||(!equation.isNull())) {
            item.x = QVariant(sx).toInt();
            item.y = QVariant(sy).toInt();
            item.equation = equation;
            items.push_back(item);
        }

        tdOld = td;
        td = tdOld->NextSiblingElement("equ");
    }

    return items;
}

void Parser::createPage(QString file, QString page) {
    XMLDocument *doc = new XMLDocument;
    doc->LoadFile(file.toStdString().c_str());

    XMLElement *root = doc->FirstChildElement("sheet");
    if (root==nullptr) {
        return;
    }

    XMLElement *newPage = doc->NewElement("page");
    newPage->SetAttribute("title",page.toStdString().c_str());
    root->InsertEndChild(newPage);

    XMLElement *newData = doc->NewElement("data");
    newPage->InsertFirstChild(newData);

    XMLElement *newMath = doc->NewElement("math");
    newPage->InsertFirstChild(newMath);

    doc->SaveFile(file.toStdString().c_str());
}

void Parser::removePage(QString file, QString page) {
    XMLDocument *doc = new XMLDocument;
    doc->LoadFile(file.toStdString().c_str());

    XMLElement *root = doc->FirstChildElement("sheet");
    if (root==nullptr) {
        return;
    }

    XMLElement *toDelete = getPageElement(root,page);
    root->DeleteChild(toDelete);

    doc->SaveFile(file.toStdString().c_str());
}

void Parser::setData(QString file, QString page, QVector<SheetItem> items) {
    XMLDocument *doc = new XMLDocument;
    doc->LoadFile(file.toStdString().c_str());

    XMLElement *root = doc->FirstChildElement("sheet");
    if (root==nullptr) {
        return;
    }

    XMLElement *pageElement = getPageElement(root,page);
    if (pageElement==nullptr) {
        return;
    }

    XMLElement *dataElement = pageElement->FirstChildElement("data");
    if (dataElement==nullptr) {
        return;
    }
    dataElement->DeleteChildren();

    for (int i = 0; i<items.size(); i++) {
        SheetItem current = items.at(i);
        QString sx = QVariant(current.x).toString();
        QString sy = QVariant(current.y).toString();

        XMLElement *td = doc->NewElement("td");
        dataElement->InsertEndChild(td);

        td->SetAttribute("x",sx.toStdString().c_str());
        td->SetAttribute("y",sy.toStdString().c_str());
        td->SetText(current.data.toStdString().c_str());

        QString fgColor = current.fgColor.name();
        if (!fgColor.isNull()) {
            td->SetAttribute("fgColor",fgColor.toStdString().c_str());
        }

        QString bgColor = current.bgColor.name();
        if (!bgColor.isNull()) {
            td->SetAttribute("bgColor",bgColor.toStdString().c_str());
        }

        QString tooltip = current.tooltip;
        if (!tooltip.isNull()) {
            td->SetAttribute("tooltip",tooltip.toStdString().c_str());
        }

        int colWidth = current.colWidth;
        QString colWidthStr = QVariant(colWidth).toString();
        td->SetAttribute("colwidth",colWidthStr.toStdString().c_str());

        int rowWidth = current.rowWidth;
        QString rowWidthStr = QVariant(rowWidth).toString();
        td->SetAttribute("rowwidth",rowWidthStr.toStdString().c_str());

        QString spanX = QVariant(current.spanx).toString();
        td->SetAttribute("spanx",spanX.toStdString().c_str());

        QString spanY = QVariant(current.spany).toString();
        td->SetAttribute("spany",spanY.toStdString().c_str());
    }

    doc->SaveFile(file.toStdString().c_str());
}

void Parser::setMathData(QString file, QString page, QVector<MathItem> items) {
    XMLDocument *doc = new XMLDocument;
    doc->LoadFile(file.toStdString().c_str());

    XMLElement *root = doc->FirstChildElement("sheet");
    if (root==nullptr) {
        return;
    }

    XMLElement *pageElement = getPageElement(root,page);
    if (pageElement==nullptr) {
        return;
    }

    XMLElement *mathElement = pageElement->FirstChildElement("math");
    if (mathElement==nullptr) {
        return;
    }
    mathElement->DeleteChildren();

    for (int i = 0; i<items.size(); i++) {
        MathItem current = items.at(i);
        QString sx = QVariant(current.x).toString();
        QString sy = QVariant(current.y).toString();

        XMLElement *td = doc->NewElement("equ");
        mathElement->InsertEndChild(td);

        td->SetAttribute("x",sx.toStdString().c_str());
        td->SetAttribute("y",sy.toStdString().c_str());
        td->SetText(current.equation.toStdString().c_str());
    }

    doc->SaveFile(file.toStdString().c_str());
}

XMLElement *Parser::getPageElement(XMLElement *root, QString title) {
    XMLElement *pageElement = root->FirstChildElement("page");
    XMLElement *oldPage;
    while (pageElement!=nullptr) {
        QString pgTitle = pageElement->Attribute("title");
        if (pgTitle==title) {
            break;
        }
        oldPage = pageElement;
        pageElement = oldPage->NextSiblingElement("page");
    }
    return pageElement;
}
