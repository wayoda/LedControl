# Controlling multiple rows

**Check the LedController for more infomation about its functions or look for the [Examples](examples.html).**

As of version 2.0.0 the LedController supports controlling multiple rows from one LedController object.
This introduction explains everything you need to know to use multiple rows.

## Data structure

First look at the image below.
It explains the data structure of the segments.
Each square represents one segment and the example has a total of 8 Segments with 4 Segments in each row.
@image html multi_row_numbering.svg "Data order of the Segements" width=50%

As you can see the 0 is in the bottom left corner (unless you rotate your matrix).
When you create a new LedController object it is required to specifiy the dimensions of the matrix, in other words the number of columns and rows.
For this example the type of the LedController would be `LedController<4,2>` because there are 4 columns and 2 rows.
These dimensions can be an arbitary size as long as you have enough memory (you will notice if you don't have enough).

## Creating a valid controller_configuration

If you have a different SPI pin for each row, you need to create a custom controller_configuration with the same dimensions as your controller.
By default it is assumed that all of the segments are connected in series in the order of their indexes.

Look at the [controller_configuration page](de/d51/md_doc_controller_configuration.html) for details on how to use all of the fiels, only multi_row_SPI will be explained here.

## Displaying data on segments
