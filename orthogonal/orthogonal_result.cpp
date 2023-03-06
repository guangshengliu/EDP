#include "orthogonal_result.h"

Orthogonal_Result::Orthogonal_Result(int factors ,int levels, QWidget *parent)
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

    for (int var = 0; var < factors; ++var) {
        charFormat.setFont(QFont("Consolas",15));
        cursor.setCharFormat(charFormat);
        cursor.insertText(QString::asprintf("Factor %d homogeneous subset:",var+1));
        charFormat.setFont(QFont("Consolas",8));
        cursor.setCharFormat(charFormat);

        QTextTable *table_factor = Three_Lines_Table(levels+1,5);
        table_factor->cellAt(0,0).firstCursorPosition().insertText("Source of variation");
        for (int i = 1; i <= levels; ++i)
            table_factor->cellAt(i,0).firstCursorPosition().insertText(QString::asprintf("Level %d",i));
        table_factor->cellAt(0,1).firstCursorPosition().insertText("Levels");
        table_factor->cellAt(0,2).firstCursorPosition().insertText("Free degree");
        table_factor->cellAt(0,3).firstCursorPosition().insertText("k-value");
        table_factor->cellAt(0,4).firstCursorPosition().insertText("Range");
        table.append(table_factor);
        result_text->append("\n");
    }
    // Set Title Font
    charFormat.setFont(QFont("Consolas",15));
    cursor.setCharFormat(charFormat);
    cursor.insertText("Analysis of variance table : ");
    charFormat.setFont(QFont("Consolas",8));
    cursor.setCharFormat(charFormat);

    QTextTable *table_anova = Three_Lines_Table(factors+2,7);
    table_anova->cellAt(0,0).firstCursorPosition().insertText("Source of variation");
    table_anova->cellAt(0,1).firstCursorPosition().insertText("Sum of squares");
    table_anova->cellAt(0,2).firstCursorPosition().insertText("Free degree");
    table_anova->cellAt(0,3).firstCursorPosition().insertText("Mean square");
    table_anova->cellAt(0,4).firstCursorPosition().insertText("F-value");
    table_anova->cellAt(0,5).firstCursorPosition().insertText("F-function");
    table_anova->cellAt(0,6).firstCursorPosition().insertText("Significant level");
    for (int var = 1; var <= factors; ++var)
        table_anova->cellAt(var,0).firstCursorPosition().insertText(QString::asprintf("Factor %d",var));
    table_anova->cellAt(factors+1,0).firstCursorPosition().insertText("Sum");
    table.append(table_anova);
}

Orthogonal_Result::~Orthogonal_Result()
{
}

/*
 * Draw three line table
*/
QTextTable* Orthogonal_Result::Three_Lines_Table(int row , int column)
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
