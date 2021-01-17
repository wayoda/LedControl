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

The example [LedControllerDemoRocketMulti](@ref LedControllerDemoRocketMulti.ino) will be used to demonstrate each of the parts that need to be done.

## Creating a valid controller_configuration

If you have a different SPI pin for each row, you need to create a custom controller_configuration with the same dimensions as your controller.
By default it is assumed that all of the segments are connected in series in the order of their indexes.

If you want each row to be connected to a different SPI pin you need to set row_SPI_CS to the correct values.
`row_SPI_CS[0]` should be set to the pin of row 0 and `row_SPI_CS[1]` to the pin of row 1.
**You have to set the pin of each row to a unique value or you might encounter issues!**
You also have to set SPI_CS to 0 or virtual_multi_row to false.
One of those two is enough but setting both makes sure that virtual_multi_row is disabled.

Below is an example of how the controller_configuration might be created for this example.

```cpp
    controller_configuration<4,2> conf;
    conf.SPI_CS = 0;
    conf.virtual_multi_row = false;

    conf.row_SPI_CS[0] = 25;
    conf.row_SPI_CS[1] = 15;
```

If you want to use virtual_multi_row (have all of the segments connected in series) then you have to set SPI_CS to you CS pin and set virtual_multi_row to true (which is the default).
As long as those conditions are satisfied, the values of row_SPI_CS will be ignored.

Look at the [controller_configuration page](controller_configuration.md) for details on how to use all of the other fiels, only row_SPI_CS was explained here.

## Displaying data on segments

Now you can display data just as in [usage information](usage.md) using `setSegment` with the coordinates of the segment.
`getConfig().getSegmentNumber` can be used to calculate the segment index which is required by the `setRow` and `setColumn` function to set a single row or column.
