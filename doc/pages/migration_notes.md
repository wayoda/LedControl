# General Migration Notes

This pages contains some basic information on the transition from one release to another and lists where changes are necessary.

## 1.x.y to 2.0.z

No matter which version you come from there are a lot of changes for version 2.0.x.
Please read the full guide [here](1.x.y_to_2.0.z.md) to learn what changes are necessary to get your code working with release 2.0.x.
Spoiler: no matter how you have written your code it needs changes since the LedController is now a template.

### Migration from 1.6.x to 1.7.x

Unless you used ByteBlock as return type and relied on that there should nothing that breaks compatibility.

As of release 1.7.x there is no support for the std data types to reduce the codebase.
If you do not want to use a pointer to an Array as a parameter to return the result, please wait for release 2.0.x where ByteBlock is implemented as a class and can be returned by functions (there are other bigger changes that make that transition a bit more complicated).
