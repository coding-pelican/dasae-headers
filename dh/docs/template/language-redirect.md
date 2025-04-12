---
title: Language Redirect Template
description: Template for creating language-specific redirects
created: 2025-04-01
updated: 2025-04-01
version: v0.1
---

# Language Redirect

This is a template for creating language-specific redirects.

## Usage

1. Make a copy of this file for each document that needs redirection
2. Place it in the appropriate language directory (e.g., `/ko/` for Korean)
3. Update the metadata section with the specific target information
4. The redirect mechanism will automatically route users to the correct language version

## Template Example

For a file like `Arr.md` with Korean translation:

```
---
title: Fixed-Size Arrays (Arr) - Korean Redirect
description: Redirects to the Korean version of the Arr module documentation
created: 2025-04-01
updated: 2025-04-01
version: v0.1.0
---

# 고정 크기 배열 (Arr)

이 페이지는 한국어 버전의 문서로 자동 리디렉션됩니다.
This page redirects to the Korean version of the documentation.
```

## Implementation

The redirection system relies on front matter metadata to properly route users to their language-preferred content while maintaining SEO benefits and proper linking structure.

### Full Example

```
---
title: Document Title - Language Redirect
description: Redirects to the [Language] version of the [Document] documentation
created: YYYY-MM-DD
updated: YYYY-MM-DD
version: vX.Y.Z
---

# [Title in Target Language]

[Brief description in target language]
This page redirects to the [Language] version of the documentation.
```
