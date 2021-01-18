# Moving the displayed data

**Check the LedController for more infomation about its functions or look for the [Examples](examples.html).**

This pages describes how to move the displayed data around and what you need to know when using the functions.
As of version 2.0.0 the movement functions are aware of the number of rows and columns and use return types with the correct size.
To see details on how to use multiple rows check [this](d7/d20/md_doc_multi_row.html) page.

All versions since 1.5.0 support some form of data movement but this page will focus on how the movement functions work as of version 2.0.0.
The functions marked as deprecated will not be explained and only exist for better compatibility.
You should not use those and start migrating to the new functions.

There are 4 directions you can move the data in: up, down, left and right.
You can either move all rows/columns or a single row/column.

The LedController.moveLeft and LedController.moveRight functions moves all rows left/right by one pixel.
If nothing is given 0 will be shifted in.
If you want to shift some data in you have to pass the parameter `const ByteRow<rows>& shiftedInColumn`.
It is an ByteRow with the length of your number of rows.
In other words if you have 1 row the type is `ByteRow<1>` or `byte[1]` and if you have 2 rows it is `ByteRow<2>` or `byte[2]` and so on.
