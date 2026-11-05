# Create from text in Umicom Media

Use **Create from text** to turn a short passage from a book, document or conversation into an editable creation brief. A brief tells a generation tool what to make. Preparing it is a local step; it does not generate an image, video or music recording.

1. Open Umicom Media and select **Create from text** in the top navigation or **File** menu.
2. Enter a project title, choose Storyboard, Image, Video or Music brief, and paste your source excerpt.
3. Add creative direction, such as the desired mood, visual style or camera movement.
4. Select **Prepare brief**, then review and edit the result in the right column. Drag the divider to resize the columns.
5. For an image or video, use **Use in AI Image** or **Use in AI Video**. The existing generation page opens with a copy of the brief. Review its settings and approve its request separately.
6. For a storyboard or music plan, enter your chosen absolute model-settings path and select **Open model connections with this brief**. Select or configure a supported local or remote text-model connection. Open chat, enter an instruction for the captured brief, review the request and send it explicitly.
7. Save useful text using the export controls and a new filename in your chosen folder. Keep source documents separately. Changing pages preserves the current draft; closing the application does not automatically save it.

If a generator already contains a prompt, Media keeps it unless you explicitly allow replacement. If the source changes, prepare the brief again before handing it to another tool. An edited brief also requires explicit replacement before it can be regenerated. You can save the displayed draft even after changing its source.

Read the [complete step-by-step guide](../../../framework/docs/learning/media-creation-briefs.html) for a worked example, model setup concepts, limits, storage and recovery. The same guide is available as [Markdown](../../../framework/docs/learning/media-creation-briefs.md).

The image and video routes use existing provider adapters. Music briefs produce text, not synthesized audio. Serving an Umicom model through a compatible chat endpoint is supported by the same connection route; direct model loading, training and fine-tuning are separate work. There is no automatic book parser, generation queue or generated-clip timeline insertion in this workflow.

Continue editing with the [native workflow guide](NATIVE_WORKFLOWS.html), [image generation guide](SEEDREAM_IMAGES.html) and [song/video planning guide](SONG_AND_VIDEO_WORKFLOWS.html).
