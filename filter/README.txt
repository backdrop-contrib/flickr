The content of this file is based on the online documentation that can be found
at https://drupal.org/node/2171503
It is recommended to read it there, as it is more detailed.

NAME
====
Flickr Filter
Part of the Flickr module. Project page at https://drupal.org/project/flickr

OVERVIEW
========
Include single Flickr images, sets or slideshows (for sets, users or groups)
into the node body using only the Flickr ID and optionally a size parameter.

FILTER CONFIGURATION
====================
The Flickr filter (called 'Flickr linker') should be added first to a text
format at admin/config/content/formats > configure
In the 'Filter processing order' it should be placed above filters that affect
image related HTML, for example AutoFloat
(https://drupal.org/project/autofloat).

FILTER SYNTAX
=============
The filter format is: [flickr-photo:id=230452326,size=s] and
[flickr-photoset:id=72157594262419167,size=m]
You find the ID within the URL of the Flickr Photo or Set page. Note the length
of the number to distinguish a photo ID from a set ID.

A number that includes '@' is a user or group ID that can be used in the filter
in the following form:
[flickr-group:id=91484156@N00] or [flickr-user:id=98518260@N02, size=y]

Note that for now only a slideshow is supported for groups and users.
To find the Flickr ID number go to http://idgettr.com (also works for groups).

The size parameter
------------------
See https://drupal.org/node/2170535#sizes

A default size can be specified on the Flickr settings page at
admin/config/media/flickr. This size gets used in case the size parameter is
omitted, for example [flickr-photo:id=230452326]. It also means you can change
the size of all images without a specified size on the site in one go.

More info at https://www.flickr.com/services/api/misc.urls.html and
https://www.flickr.com/help/faq/search/?q=sizes

Float photos
------------
If a class or style parameter in the Flickr Filter tag
contains the term 'float' (case insensitive) it will be applied to the flickr-
wrapper span element that includes also the caption. Some CSS is added to target
by default the classes 'floatleft' and 'floatright'.

Use:
[flickr-photo:id=9247386562, class=floatleft]
This is prefered as it avoids inline styling.

Also this will work:
[flickr-photo:id=9247388074, size=m, style=float:right;]

If you enable the AutoFloat module (recommended), it should precede over the
existing 'floatleft' and 'floatright' classes found, but not over the inline
styles. Thus inline styling is a way to 'overwrite' AutoFloat's behaviour.

Adding multiple 'class' or 'style' values
-----------------------------------------
To pass multiple classes or styles the syntax has to look like: [flickr-
photo:id=9247386562, size=m, class=foo bar, style=float:left; border:solid 2px;]
Thus without quotes, but with semicolons between styles.
