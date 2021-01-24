# Moving the displayed data

**Check the LedController for more infomation about its functions or look for the [Examples](examples.html).**

This pages describes how to move the displayed data around and what you need to know when using the functions.
As of version 2.0.0 the movement functions are aware of the number of rows and columns and use return types with the correct size.
To see details on how to use multiple rows check [this](@ref multi_row) page.

All versions since 1.5.0 support some form of data movement but this page will focus on how the movement functions work as of version 2.0.0.
The functions marked as deprecated will not be explained and only exist for better compatibility.
You should not use those and start migrating to the new functions.

There are 4 directions you can move the data in: up, down, left and right.
You can either move all rows/columns or a single row/column.

## Moving a single Row/Column

LedController.moveRowLeft and LedController.moveRowRight move a single Row left/right.
The required Argument is the number of the row that should be shifted and the byte that should be shifted in.
It returns the byte that was shifted out.
The bytes that are shifted in and out have the LSB (least significant bit) on the bottom.

LedController.moveColumnDown and LedController.moveColumnUp move a single Column up/down.
The required Argument is the number of the column that should be shifted and the byte that should be shifted in.
It returns the byte that was shifted out.
The bytes that are shifted in and out have the LSB (least significant bit) on the right.

## Moving all Rows/Columns

The LedController.moveLeft and LedController.moveRight functions moves all columns left/right by one pixel.
If nothing is given 0 will be shifted in.
If you want to shift some data in you have to pass the parameter `const ByteRow<rows>& shiftedInColumn`.
It is an ByteRow with the length of your number of rows.
In other words if you have 1 row the type is `ByteRow<1>` or `byte[1]` and if you have 2 rows it is `ByteRow<2>` or `byte[2]` and so on.
Index 0 of the row corresponds to columns 0 of the Matrix, in other words the bottom segment is first.
The function will retun a ByteRow with the same length as the input.
The returned ByteRow is the Column that was shifted out as because of the movement.

The LedController.moveUp and LedController.moveDown functions moves all rows up/down by one pixel.
If nothing is given 0 will be shifted in.
If you want to shift some data in you have to pass the parameter `const ByteRow<columns>& shiftedInColumn`.
It is an ByteRow with the length of your number of columns.
In other words if you have 1 column the type is `ByteRow<1>` or `byte[1]` and if you have 2 column it is `ByteRow<2>` or `byte[2]` and so on.
Index 0 of the row corresponds to row 0 of the Matrix, in other words the left segment is first.
The function will retun a ByteRow with the same length as the input.
The returned ByteRow is the Row that was shifted out as because of the movement.

Check [LedControllerDemoRocketMulti.ino](@ref LedControllerDemoRocketMulti.ino) to see how this can be implemented.
