NAME
====
Flickr
Project page at https://drupal.org/project/flickr

REQUIREMENTS
============
This module requires Drupal 7.X and a Flickr API key.

OVERVIEW
========
The Flickr module embeds images hosted on Flickr with a link to the
corresponding Flickr page.

- Flickr images can be displayed in blocks, within content or as fields.
- Open Flickr photos in Colorbox, Lightbox or alike.
- Image caption with the Flickr title, date taken, where  and by who.
- Show the longer Flickr description on hover if it exists.

INSTALLATION AND CONFIGURATION
==============================
1.) Extract the download package in the '/sites/all/modules directory'.
2.) Enable the module in 'admin/modules'.
3.) Configure the API Key and settings at '/admin/config/media/flickr'.
4.) Allow permissions '/admin/people/permissions'.

SETTINGS
========
Settings are found at '/admin/config/media/flickr'. After changing many of the
settings below, it is often required to CLEAR THE SITE CACHE to have the changes
to take effect on existing content at '/admin/config/development/performance'.

API Key
-------
Get an API Key at http://www.flickr.com/services/apps/create/apply.

API Shared Secret
-----------------
Default Flickr User ID
To use if in the block configuration no user is specified. Must be a valid
Flickr User ID, username or email address. Emails or usernames (that might
change at any given time) are internally (in the variable) replaced with a
stable NSID (Flickr ID number). These are rendered in admin forms (settings or
block config) as Flickr usernames. 'Maria Emanuela' is human readable,
'43937662@N05' is not. Internal the module still uses the Flickr ID number as it
is more reliable.

Update interval
---------------
The refresh interval how often to check if cached Flickr API calls are up to
date. Defaults to 1 hour.

Number of photos per photoset
-----------------------------
Defaults to 20.

Default size
------------
Only if the submodule Flickr Filter is enabled.
A default Flickr size to use. This will be used when no size is specified, for
example [flickr-photo:id=3711935987]. TAKE CARE, n (320px) and c (800px) sizes
are missing on many "older" Flickr images!
Defaults to m: 240 px on longest side.

Use flickr.css
--------------
Uncheck to take care of the styling yourself in custom CSS.
Selected by default.

FLICKR INFO TO USE (field group)
--------------------------------
Minimum width to display a title caption
----------------------------------------
Small images have little space for a title caption. Replaces it with the text
"Flickr" that links to the photo page on Flickr to comply with their Guidelines.
Defaults to 100 px. Set it to 0 px to always include or 999 px to always
exclude.

Minimum image width to display date, location, photographer and optionally
license info under the caption
--------------------------------------------------------------------------------
Suppress extra info on small images. After saving the configuration clear the
cache.
Defaults to 150 px. Set it to 0 px to always include or 999 px to always
exclude.
Adds date, location and photographer info to the caption on images of a certain
width. The date is in the form of 'time ago'. The photographer's Real Name is
used, if not available the Username. In the caption it links to the user page on
Flickr.

License info in caption
-----------------------
Depending on 'Minimum image width' above.
Checkbox (not selected by default).
Used is the nonintrusive icons font that can be found at
http://creativecommons.org/about/downloads. It links to the corresponding
Creative Commons human friendly info page.
It is not necessary, but if desired you can download it, put it in your theme
folder and substitute the default used remote source that can be found in the
module's 'flickr.css' file.

Info to include when enlarging the image in Colorbox, Lightbox or alike
-----------------------------------------------------------------------
Usually this info is displayed under the enlarged image.
Checkboxes (multiple options can be selected).
- Title
- Date, location and photographer
- Description
- License info
All selected by default except for 'License info'. This info is concatenated
with '-' separators and added as the link 'title' attribute. HTML is stripped,
double quotes are removed (it goes into the title="[description]" attribute) and
special characters are decoded (&copy; turns into ©). If the title and
description are identical, only the title is used.

As a side note: The description is always included as the image 'title'
attribute (shows on hover). If the description is not available the title will
be used.

OVERLAY BROWSER SETTINGS (field group)
--------------------------------------
Colorbox, Lightbox (use the dev version) or alike.
Leave these fields empty to link directly to the Flickr photo page instead of
opening the bigger version of the image. It also omits the caption in that case.
This is only done to not alter the behaviour on previous installs of the Flickr
module. The best way of giving attribution is by using a full version of the
caption (default settings).

class
-----
Use 'colorbox' if you are using Colorbox. (https://drupal.org/project/colorbox)
With Lightbox you can leave it empty. (https://drupal.org/project/lightbox2)

rel
---
Use 'gallery-all' if you are using Colorbox. It can be anything you want,
really. As long there is something.
Use 'lightbox[gallery]' if you are using Lightbox2. The part inside the square
brackets can be anything you want but must be one word without spaces.

NOTE: With Lightbox if you select an 'Automatic image handling' for Flickr
images, you override the behaviour set here. If you don't want that, leave it
'Disabled'. (/admin/config/user-interface/lightbox2/automatic)

Image size to open
------------------
The image size to open in the overlay browser when clicking the image. Larger
sizes make navigating to next and previous pictures slower. TAKE CARE, n (320px)
and c (800px) sizes are missing on many "older" Flickr images!
Defaults to -: 500 px on longest side.

MORE INFO
=========
The flickr module uses XML-RPC to connect to Flickr's API and retrieve photo
information. Flickr API Documentation: http://www.flickr.com/services/api
