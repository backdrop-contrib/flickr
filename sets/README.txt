The content of this file is based on the online documentation that can be found
at https://drupal.org/node/2226579

As it is most likely you want to display a series of images as an album (aka
set), enable the submodule Flickr Sets. Sets are the most convenient way to
organize your images. 'Sets' are better supported by the Flickr module than
'tags'.

A photo (or video) can make part of more than one set, without the need of
duplicating it. Just go to https://www.flickr.com/photos/organize, click 'Add to
album' and drag your images into it.

Once that's done you can display the set as a thumbnail gallery or a slideshow
in:
- the node body using Flickr Filter
- a block using Flickr Block
- a field using Flickr Field.

Video support
=============
To 'play' the videos embedded on a site, instead of just showing a still image
of it:
- Put the videos (one or more) you want to display together in a set.
- Show the set in the node body, a block or a Flickr field using the size x for
  a Flash slideshow or y for the non-Flash version.

Disabling the Flickr Sets submodule: What happens?
==================================================
Only the first image of a set will be displayed in some blocks, in the photoset
filter tag and the Flickr photo field. Those will link to the set on Flickr when
clicking on them.

Flickr Blocks affected
----------------------
- User recent photosets
- Recent photosets by user
All the others are unaffected.

Flickr Filter tags affected
---------------------------
Only photoset tags like [flickr-photoset:id=72157634563269642,size=t]
are affected, but not for the sizes x and y (slideshow).

Flickr Field affected
---------------------
The Flickr Field 'photo' is only affected for the 'Item Type': 'Photoset'.
The Flickr Field 'photoset' always displays only the primary image (except for
slideshows), thus is unaffected.
