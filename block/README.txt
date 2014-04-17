The content of this file is based on the online documentation that can be found
at https://drupal.org/node/2171249
It is recommended to read it there, as it is more detailed.

NAME
====
Flickr Block
Part of the Flickr module. Project page at https://drupal.org/project/flickr

OVERVIEW
========
Flickr images or slideshows in blocks to show for example recent or random images from a
specific user, set or group.

- Configure the size, media type (photo/video) and number of images to show.
- With visibility by role or path.
- Blocks can be placed in any theme region.

There are three blocks that show the Flickr photos of a corresponding user
'dynamically' and appear on user profile pages and on posts written by the user.
To avoid that, change the block settings to exclude 'node/*' pages or show them
only on 'user/*' pages.


Of course you can do it also the other way around to show a block only on a node
page. You can fine-grain it even more by selecting only a certain content type
you want to show them on, for example blog posts.

- User recentFlickr photos
- User recent Flickr photosets
- User random Flickr photos

The block visibility setting for 'user' blocks are not only restricted by the
user in the block configuration but also by the module. The block gets displayed
when the first argument of the path is 'user' or 'node' and a specific uid is
available. This is however not visible for the web admin and confusing.
See issue #1122564: Sync the module defined visibility setting with the block
configuration. In the future these blocks will probably move to a dedicated
Flickr Profile sub-module.

Furthermore nine sitewide blocks are available. A user, photoset or group needs
to be specified in the block configuration.
- Recent photos from a Flickr user
- Recent photosets from a Flickr user
- Random photos from a Flickr user
- Recent photos from a Flickr group
- Random photos from a Flickr photoset
- Recent photos from a Flickr photoset
- Favorite public photos from a Flickr user
- Random photos from a Flickr group
- Random photos with a specific tag from a Flickr user

BLOCK CONFIGURATION
===================

Fields in the block settings that apply on all blocks.

Show n photos
-------------
The block will display this many photos.

Size of photos
--------------
Select the size of photos you'd like to display in the block.
NOTE:
For square images ('s': 75px and 'q': 150px) no real width needs to be fetched,
giving it a slight performance advantage over other sizes. Recommended if you
include many images.

Media type
----------
- all
- photos (default)
- videos
Note that for videos only an image representing the video is showed.


All '..from user' blocks.

Flickr User ID
--------------
A valid Flickr User ID, username or email address. If this is left blank, the
site's default user will be used if set.
Emails or usernames (that might change at any given time) are internally (in the
variable) replaced with a stable NSID (Flickr ID number). These are rendered in
this field as Flickr usernames. 'Maria Emanuela' is human readable,
'43937662@N05' is not. Internal the module still uses the Flickr ID number as it
is more reliable.


Other fields are self-explaining.

Sitewide block settings
=======================

Generic block configuration settings can be found at
'admin/config/media/flickr'.

BLOCK SETTINGS
--------------
Only if the submodule Flickr Block is enabled.

Show a Flickr counter
---------------------
Shows under the block title how many photos are displayed out of the total number available for a user, group or set on Flickr.

Extra links to Flickr maps
--------------------------
Include extra links to maps available for a user, group or set on Flickr. Locations mentioned (if displayed) under individual images in any case link to corresponding Flickr user maps.

Refresh rates
-------------
It is recommended to set two different refresh values for random (default 23 hours) and other blocks (default 31 hours) to avoid they refresh all together. Lower values impact performance negatively. Consider to use cache warming. See https://drupal.org/node/1576686 (Load Page Cache after cron Runs).
