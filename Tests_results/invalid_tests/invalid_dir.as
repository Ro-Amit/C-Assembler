; Invalid .data and .string

.data 5,,6
.data 9, ten, 3
.data 100,
.data
.string abcde
.string "hello
.string hello"
.string ""
.string "after" here
.data 1, 2,3    here
.extern
.extern LOCAL
LOCAL: .data 1
.entry
.entry NotHere