IMPROVEMENTS:
 - [ ] GFX_read_texture function should check if reading from file was sucessful (if not such things
   happen, some strange value is passed over to another part of the code, resulting in strange, hard
   to disect errors). Furthermore, any method using this function should working, despite the fact
   that animation sheet (or texture if that matter) was not provided - for example, fully colored
   pink rect should be created on a fly and use as "no texture", texture.

- [ ] debug rays function wont work properly on new engine version
- [ ] investigate memory leaks in light.c (please seek profiling/memcheck.txt)
- [ ] implement level structure v2, as the v1 is messy and buggy
