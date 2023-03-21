#include "uniform_result.h"

Uniform_Result::Uniform_Result(int factors ,int levels, QWidget *parent)
    :QWidget(parent)
{
    layout = new QHBoxLayout(this);
    result_text = new QTextEdit;
    layout->addWidget(result_text);
    result_text->setReadOnly(true);
    //  Set Font
    QTextDocument *doc = result_text->document();
    QFont font("Consolas",8);
    doc->setDefaultFont(font);
    //  Set Title
    QTextCharFormat charFormat;
    QTextCursor cursor = result_text->textCursor();
    charFormat.setFont(QFont("Consolas",15));
    cursor.setCharFormat(charFormat);
    cursor.insertText("Summary of regression model:");
    charFormat.setFont(QFont("Consolas",8));
    cursor.setCharFormat(charFormat);
    QTextTable *table_summary = Three_Lines_Table(2,4);
    table_summary->cellAt(0,0).firstCursorPosition().insertText("Model");
    table_summary->cellAt(1,0).firstCursorPosition().insertText("Regression-Model");
    table_summary->cellAt(0,1).firstCursorPosition().insertText("R-Squares");
    table_summary->cellAt(0,2).firstCursorPosition().insertText("Adjust-R-Squares");
    table_summary->cellAt(0,3).firstCursorPosition().insertText("Standard error of estimate");
    result_text->append("\n");
    table.append(table_summary);

    // Set Title Font
    charFormat.setFont(QFont("Consolas",15));
    cursor.setCharFormat(charFormat);
    cursor.insertText("Analysis of variance table : ");
    charFormat.setFont(QFont("Consolas",8));
    cursor.setCharFormat(charFormat);
    QTextTable *table_anova = Three_Lines_Table(4,7);
    table_anova->cellAt(0,0).firstCursorPosition().insertText("Model");
    table_anova->cellAt(1,0).firstCursorPosition().insertText("Regression");
    table_anova->cellAt(2,0).firstCursorPosition().insertText("Residual error");
    table_anova->cellAt(3,0).firstCursorPosition().insertText("Total");
    table_anova->cellAt(0,1).firstCursorPosition().insertText("Sum of squares");
    table_anova->cellAt(0,2).firstCursorPosition().insertText("Free degree");
    table_anova->cellAt(0,3).firstCursorPosition().insertText("Mean square");
    table_anova->cellAt(0,4).firstCursorPosition().insertText("F-value");
    table_anova->cellAt(0,5).firstCursorPosition().insertText("F-function");
    table_anova->cellAt(0,6).firstCursorPosition().insertText("Significant level");
    table.append(table_anova);
}

Uniform_Result::~Uniform_Result()
{
}

/*
 * Draw three line table
*/
QTextTable* Uniform_Result::Three_Lines_Table(int row , int column)
{
    QTextCursor cursor(result_text->textCursor());
    cursor.movePosition(QTextCursor::End);
    QTextTable *table = cursor.insertTable(row, column);

    QTextTableFormat tableFormat = table->format();
    tableFormat.setWidth(QTextLength(QTextLength::FixedLength,result_text->width()*0.8));
    tableFormat.setBorderCollapse(true);
    tableFormat.setBorderStyle(QTextFrameFormat::BorderStyle_None);
    //  Set each column equal
    QVector<QTextLength> colLength = tableFormat.columnWidthConstraints();
    for (int i = 0; i < column; ++i) {
        colLength.append(QTextLength(QTextLength::FixedLength,tableFormat.width().rawValue()/column));
    }
    tableFormat.setColumnWidthConstraints(colLength);

    table->setFormat(tableFormat);

    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < column; ++j) {
            QTextTableCell cell = table->cellAt(i,j);
            if(i == 0)
            {
                QTextTableCellFormat cell_format;
                cell_format.setTopBorderStyle(QTextFrameFormat::BorderStyle_Solid);
                cell_format.setBottomBorderStyle(QTextFrameFormat::BorderStyle_Solid);
                cell.setFormat(cell_format);
            }
            else if(i == row-1)
            {
                QTextTableCellFormat cell_format;
                cell_format.setBottomBorderStyle(QTextFrameFormat::BorderStyle_Solid);
                cell.setFormat(cell_format);
            }
            // Set cell text in center
            QTextCursor cellCursor = cell.firstCursorPosition();
            QTextBlockFormat cellBlockFormat = cellCursor.blockFormat();
            cellBlockFormat.setProperty(QTextFormat::BlockAlignment,Qt::AlignCenter);
            cellCursor.setBlockFormat(cellBlockFormat);
        }
    }
    return table;
}
