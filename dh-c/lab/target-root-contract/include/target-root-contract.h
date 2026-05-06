#ifndef TARGET_ROOT_CONTRACT_H
#define TARGET_ROOT_CONTRACT_H

#if defined(_WIN32)
#define TRC_EXPORT __declspec(dllexport)
#else
#define TRC_EXPORT
#endif

int trc_core(void);
int trc_feature(void);
int trc_internal(void);
int trc_total(void);
TRC_EXPORT int trc_plugin_total(void);

#endif /* TARGET_ROOT_CONTRACT_H */
