#include "C.h"
#include "dh/main.h"
#include "dh/io/stream.h"

fn_((dh_main(S$S_const$u8 args))(E$void) $scope) {
    let_ignore = args;
    io_stream_println(u8_l("Hello, world!"));
    B_init();
    let next = B_next();
    io_stream_println(u8_l("B_next: .key={:ul}, .value={:fl}"), next.key, next.value);
    io_stream_println(u8_l("added: {:fl}"), as$(f64)(next.key) + next.value);
    io_stream_println(u8_l("B_added: {:fl}"), B_added());
    io_stream_println(u8_l("B_len: {:ul}"), B_len());
    return_ok({});
} $unscoped_(fn);

/*
PS C:\dasae\dev\dasae-headers\dh-examples\subframes> dh-c build --args="-Dengine_core_VT100Ex_comp_sharded"
Using DH library from environment variable: C:\dasae\dev\dasae-headers\dh
Detected project: subframes at C:\dasae\dev\dasae-headers\dh-examples\subframes
Compiling dependencies 'subframes':
- engine
Compiling dependencies 'engine':
- engine-stub/VT100Ex
- my-test-module
- stb/image
Compiling library: engine-stub/VT100Ex
Created directory: C:\dasae\dev\dasae-headers\dh-examples\engine-stub\VT100Ex\lib\.obj
Successfully compiled library: engine-stub/VT100Ex -> C:\dasae\dev\dasae-headers\dh-examples\engine-stub\VT100Ex\lib\VT100Ex.lib
Compiling dependencies 'my-test-module':
- module-a
- module/another-module-b
Compiling library: module-a
Created directory: C:\module-a\lib\.obj
Successfully compiled library: module-a -> C:\module-a\lib\module-a.lib
Compiling library: module/another-module-b
Created directory: C:\module\another-module-b\lib\.obj
Successfully compiled library: module/another-module-b -> C:\module\another-module-b\lib\another-module-b.lib
Bring dependencies 'my-test-module':
- 'my-test-module\lib\deps\module-a' <- 'module-a'
- 'my-test-module\lib\deps\module\another-module-b' <- 'module/another-module-b'
Compiling library: my-test-module
Created directory: C:\dev-test\my-test-module\lib\.obj
Successfully compiled library: my-test-module -> C:\dev-test\my-test-module\lib\my-test-module.lib
Compiling library: stb/image
Created directory: C:\dasae\dev\dasae-headers\dh-examples\heightmap-horizon-space\lib\stb\image\lib\.obj
Successfully compiled library: stb/image -> C:\dasae\dev\dasae-headers\dh-examples\heightmap-horizon-space\lib\stb\image\lib\image.lib
Bring dependencies 'engine':
- 'engine\lib\deps\engine-stub\VT100Ex' <- 'engine-stub/VT100Ex'
- 'engine\lib\deps\module-a' <- 'module-a' from 'my-test-module'
- 'engine\lib\deps\module\another-module-b' <- 'module/another-module-b' from 'my-test-module'
- 'engine\lib\deps\my-test-module' <- 'my-test-module'
- 'engine\lib\deps\stb\image' <- 'stb/image'
Compiling library: engine
Created directory: C:\dasae\dev\dasae-headers\dh-examples\engine\lib\.obj
Successfully compiled library: engine -> C:\dasae\dev\dasae-headers\dh-examples\engine\lib\engine.lib
Bring dependencies 'subframes':
- 'subframes\lib\deps\engine-stub\VT100Ex' <- 'engine-stub/VT100Ex' from 'engine'
- 'subframes\lib\deps\module-a' <- 'module-a' from 'my-test-module'
- 'subframes\lib\deps\module\another-module-b' <- 'module/another-module-b' from 'my-test-module'
- 'subframes\lib\deps\my-test-module' <- 'my-test-module' from 'engine'
- 'subframes\lib\deps\stb\image' <- 'stb/image' from 'engine'
- 'subframes\lib\deps\engine' <- 'engine'
Compiling project: subframes
Created directory: C:\dasae\dev\dasae-headers\dh-examples\subframes\build\.obj
Successfully compiled project: subframes -> C:\dasae\dev\dasae-headers\dh-examples\subframes\build\subframes.exe
Build successful!
*/
