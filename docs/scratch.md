
Compiled language - with the cli able to do project management and tests

Create a project directory
```
astro new app my_proj
```

Add module to binary
```
astro add mod my_proj module_a
```

Compile a project
```
astro build my_proj
```

Run a project (build and execute)
```
astro run my_proj
```

Test a project
```
astro test my_proj
```

Run a binary
```
astro target.bin
```


# Ideaing


```

my_proj
  |
  |-- module_a
  |     |
  |     |-- mod.ast
  |     |-- file.ast
  |     |-- file1.ast
  |     
  |-- module_b
  |     |
  |     |-- mod.ast
  |     |-- file.ast
  |
  |-- src
  |    |
  |    |-- main.ast
  |    |-- some_file.ast
  |
astro.toml

```

```

[package]
authors = ["josh bosley"]
title = "my_proj"
description = "A demo project"
license = "MIT"
version = "0.1.0"

[bin]
name = "my_project_cli"
path = "src"
modules = ["module_a/mod.ast", "module_b/mod.ast"]


```