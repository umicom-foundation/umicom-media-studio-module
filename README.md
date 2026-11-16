# umicom-media-studio-module
Thin C23 Umicom Media Studio (Video Studio product experience) application composition over Umicom Framework

## Review linked context values

Media Studio exposes Framework's reviewed context changes through
`umi_media_studio_workspace_context_review`, `umi_media_studio_workspace_context_apply`
and `umi_media_studio_workspace_clear_context` in
`umicom/media_studio/workspace_commands.h`. A context group is a named value that
related panels can share; for example, a host could use `media.project` with the sample
value `sample-film`. The host must connect that name to its panel consumers.

1. Use a runtime initialised with this product's canonical experience. Prepare
   the requested changes with the review function; preparation changes no live state.
2. Display the copied Framework summary and rows. Keep the runtime and its
   workbench alive while the user reviews the proposed values. If the summary
   reports UI differences, show the captured UI value beside the cached value.
3. Apply only after acceptance, then destroy the review. If the workspace changed,
   prepare a fresh review. Cancelling only destroys the review.

This is a module API; native review screens are separate host work. Context edits
do not execute product commands or external operations. The shared guide at
`framework/docs/guides/REVIEWING_LINKED_CONTEXTS.html` in the Applications checkout
explains capacity, ownership, thread coordination and recovery in more detail.

The [local asset capture guide](docs/LOCAL_ASSETS.html) explains the shared Assets page: select a complete local file, keep its bytes in memory and optionally write a new copy. Captures are not yet saved as project attachments or sent to AI providers.

Avatar video workflow: see [the step-by-step guide](docs/AVATAR_VIDEOS.html).

Image generation workflow: see [Create a Seedream image](docs/SEEDREAM_IMAGES.html) for request review, local credentials, preview, export and recovery.

The [song and video workflow guide](docs/SONG_AND_VIDEO_WORKFLOWS.html) introduces local timed lyrics, beat-aligned shot plans, SRT captions and reviewed PixVerse video requests.

Keep tutorial screenshots, recorded narration and reference documents together before editing. The **Creative workbench → Asset library** page imports complete files, reorders them, saves and reopens a portable collection, and exports selected assets. It is separate from the scene project and never uploads files. See the [shared asset library guide](../../framework/docs/learning/creative-asset-libraries.html) for limits, save steps and recovery.

The **Audio arrangement** page combines supported PCM WAV assets with timed placements, source trimming, gain and fades. Render in memory, inspect clipping, then export a new stereo WAV. Save the editable arrangement separately from its asset library. See [Arrange audio clips](../../framework/docs/learning/audio-arrangements.html) for the supported formats and complete workflow.

On **Audio clip**, render a range, choose **Load preview for listening**, then press Play. Editing the range retires the player; hiding the page pauses it. Listening volume does not change export gain. See [Listen to audio previews](../../framework/docs/learning/listen-to-audio-previews.html) for supported WAVE files, backend setup and recovery.

## Native workflow guide

Read the [native workflow guide](docs/NATIVE_WORKFLOWS.html) for step-by-step use of the implemented controls, expected results and current limits.

## Create media from text

The **Create from text** workspace prepares an editable brief from a book excerpt,
document or conversation. It passes reviewed text to the existing AI Image,
AI Video or local/remote model-connection tools while preserving their request
review and approval steps. Read [Create from text](docs/CREATE_FROM_TEXT.md) for
the steps, explicit save locations and supported outputs. Music briefs are
written plans; they do not synthesize audio.

## Independent installed Framework SDK

Configure the module directly with an absolute `UMICOM_SDK_ROOT` and an SDK
exporting the application composition helpers and creative component. This mode
does not require a sibling Framework checkout. The Applications parent still
supplies source-built targets in the normal ecosystem build.

```powershell
cmake -S . -B build/sdk-debug -G Ninja -DUMICOM_SDK_ROOT=C:/SDKs/Umicom -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTING=ON
cmake --build build/sdk-debug
ctest --test-dir build/sdk-debug --output-on-failure
```

Stop on errors and use the SDK's matching compiler/dependency environment.
Read `share/umicom/framework/docs/learning/sdk-product-composition.html` in the
installed SDK for package requirements, deployment and recovery. These commands
are instructions to execute and record, not evidence of a completed build.


## Inspect saved ComfyUI workflows

The native console accepts `comfy inspect CONNECTION WORKFLOW OBJECT_INFO`
with three absolute paths. It delegates to Framework's `comfy_protocol` SDK
component, which reads bounded local files and reports missing classes and
required field names as JSON. Use `comfy --help` for syntax. No arguments still
show the existing product composition report.

The report labels its origin as offline files. Exit 0 means required names were
observed; exit 3 means missing names, with a complete report; exit 2 means a usage
error; exit 1 means reading, parsing, identity or output failed. These observations
do not establish type correctness, model availability, current settings or
permission to generate media. This command performs no network or model action.

Use the installed SDK's `share/umicom/framework/examples/comfy_inspection`
fixtures for the first exercise. Read the SDK's ComfyUI course, lessons 12 and 13,
for exact commands, output interpretation and recovery. Synthetic fixture classes
do not claim to be installed ComfyUI nodes. Native console and installed SDK
qualification remain necessary before relying on this workflow.


### Review reference candidates from captured files

The same console now supports `comfy references CONNECTION WORKFLOW OBJECT_INFO` with optional `--client-id ID`, `--offset N` and `--limit N`. Use the three absolute file paths described above. For example, after building the console:

```powershell
& "C:/path/to/umicom-media-studio-console.exe" comfy references "C:/ComfyReview/connection.umiconnection" "C:/ComfyReview/workflow.json" "C:/ComfyReview/object-info.json" --limit 20
```

Replace the executable path with your verified build output. The JSON response labels its origin as offline files and includes complete aggregate counts plus a bounded page. Follow `next_offset` while `remaining_count` is nonzero; require matching connection/request/catalogue digests across pages. Unknown slot and output-count fields are null rather than fabricated zeros.

Exit 0 means that an observation page was produced, including unresolved candidates. Exit 2 means usage error; exit 1 means read, parse, identity, page or output failure. This differs deliberately from the existing required-name inspection command. Candidate strings are printed and may contain sensitive custom-node data; unrelated values are omitted, but no secret-redaction guarantee is made.

All parsing, file admission and pagination remain in Framework. This route neither contacts a server nor validates an executable graph. Use the Framework ComfyUI course, lesson 16, for the installed SDK example and recovery steps. Build, native tests, installed SDK operation and application behavior require qualification with the selected Framework package.

### Inspect captured model progress

Media delegates offline Ollama pull-transcript inspection to Framework:

```powershell
& "C:/path/to/umicom-media-studio-console.exe" models inspect-pull "C:/ModelReview/pull.ndjson"
```

Use a verified executable and an absolute regular UTF-8 file. The command does not contact a server or download a model. It emits byte/event counts, the observed file SHA-256 and the provider-reported outcome; server identity and model installation remain unverified. Provider messages, model names and file paths are omitted from the JSON. Exit 0 means reported success, 3 means a complete provider-error transcript, 2 means usage error and 1 means file/protocol/output failure. Truncated or malformed transcripts produce no report.

See lesson 7 of the Framework SDK model-progress course for a reproducible fixture, the public C API, limits and troubleshooting. The same fixture driver qualifies Media, native CLI and the independent SDK example. Those native checks require execution against the selected SDK; source wiring and configuration alone are not a runtime pass.


### Inspect a captured ComfyUI queue

The Media console uses Framework's shared captured-queue reader:

~~~powershell
.\umicom-media-studio-console.exe comfy queue --help
.\umicom-media-studio-console.exe comfy queue C:\examples\connection.umiconnection C:\examples\queue.json --offset 0 --limit 32
~~~

Supply absolute paths to a canonical saved provider document and captured
queue JSON. This command prints an offline page of running/pending IDs, exact
scheduling-number strings, counts and content digests. It does not contact a
server, generate media, interrupt jobs or change queue contents. A missing ID
does not establish completion, and pending response order is not execution
rank.

Separate invocations reread both files. Compare the response digest, connection
fingerprint and historical revision before combining pages. IDs and digests may
be sensitive; omitted workflow contents are not a general redaction guarantee.
Exit 0 means a complete observation page, 2 means invalid arguments, and 1 means
a file, parse, page or output failure.

The Framework SDK's ComfyUI course includes an independent queue-client project
and exercises. The shared native qualification cases are registered under
~~~text
media.comfy.queue.
~~~
They compare Media with the native CLI when both are built. Source changes and
test registration do not establish a passing native or GUI result.


### Inspect captured ComfyUI system information

The Media console can inspect a saved system report through the same Framework
service used by the native CLI and installed-SDK example:

~~~powershell
.\umicom-media-studio-console.exe comfy system --help
.\umicom-media-studio-console.exe comfy system C:\examples\connection.umiconnection C:\examples\system.json
~~~

Supply an enabled ComfyUI loopback connection document and a reviewed UTF-8
capture of the server's system information. Both paths must be absolute. The
reader admits regular files, a bounded connection document, and up to 1 MiB of
response JSON containing at most 64 devices. It does not contact a provider,
load a model, allocate device memory or generate media.

The JSON output labels its origin as offline files. Unknown metadata becomes
JSON null; known memory counters, device indices and connection revisions are
decimal strings so their digits survive other JSON runtimes. A reported free
count greater than its total is retained and flagged. These observations do not
promise that a model fits, that memory is reserved, or that the capture is fresh.
RAM, VRAM and framework-specific counters may overlap.

Exit 0 means the report was emitted, 2 means incorrect arguments, and 1 means a
file, parse or output failure. Check PowerShell's $LASTEXITCODE immediately after
the command and stop on errors. Keep captured reports private when their names
or system metadata are sensitive. Launch arguments and opaque extension fields
are not emitted, but metadata filtering is not a general secret-redaction service.

The Framework learning guide **ComfyUI system reports with the Umicom SDK**
includes a complete independent C23 project and captured-file lessons.
Qualification registers media.comfy.system.* and can compare the Media console
with the native CLI when both targets are available. Registration alone does
not establish that either executable has passed its tests.


## Browse captured ComfyUI nodes

The native console also accepts:

~~~powershell
.\umicom-media-studio-console.exe comfy nodes --help
.\umicom-media-studio-console.exe comfy nodes C:/NodeReview/connection.umiconnection C:/NodeReview/object-info.json --search image --order class --offset 0 --limit 20
~~~

Framework owns file admission, copied catalogue selection and JSON pages. Search
matches class/display/category text with ASCII case folding. Category filters
respect slash boundaries. A flag absent from the capture stays null; explicitly
false filters do not include unknown flags. These are node declarations, not a
list of installed model weights or proof of execution readiness.

Before joining pages, compare connection revision plus connection, response and
selection digests. Follow next_offset only while remaining_count is nonzero.
Changing filters or order can change selection identity even with the same input
file. The command performs no provider request, custom-node import or generation.
It does not change the existing no-argument composition report or GUI layout.

Shared media.comfy.nodes.* regression cases compare the console with the native
CLI when both targets are present. Native execution and installed SDK behavior
remain separately qualified. The Framework ComfyUI course includes an independent
SDK browser project and the Streamlit course explains captured table presentation.
