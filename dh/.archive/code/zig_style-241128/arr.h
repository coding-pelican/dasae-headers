#define Arr(TItem, val_len)   \
    struct {                  \
        TItem items[val_len]; \
    }

#define Arr_at(self, val_idx) (self->items + val_idx)
#define Arr_len(self)         (sizeof(self->items) / sizeof(self->items[0]))
