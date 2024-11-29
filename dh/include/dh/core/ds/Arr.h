// Array
#define Arr(TItem, val_len)   \
    struct {                  \
        TItem items[val_len]; \
    }
#define Arr_new(TItem, val_len) ((Arr(TItem, val_len)){ 0 })
#define Arr_get(self, i)        ((self).items[i])
#define Arr_set(self, i, val)   ((self).items[i] = (val))
#define Arr_len(self)           (sizeof((self).items) / sizeof((self).items[0]))
