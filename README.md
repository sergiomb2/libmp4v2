# mp4v2

This is a fork of mp4v2 from https://code.google.com/archive/p/mp4v2/.

Changes here are in compliance with the Mozilla Public License.

-------
<pre>
All docs are located in doc/ subdirectory. Useful starting points:

Release Notes           -- doc/ReleaseNotes.txt
Building the Source     -- doc/BuildSource.txt
Building the Repository -- doc/BuildRepository.txt

See the COPYING file for license rights and limitations

From https://git.busybox.net/buildroot/commit/?id=0a860f21e1b8004ee937c20d54d29a5e66f96651
- Fix the following CVEs:
  - CVE-2018-14054: A double free exists in the MP4StringProperty class
    in mp4property.cpp in MP4v2 2.0.0. A dangling pointer is freed again
    in the destructor once an exception is triggered.
    Fixed by
    https://github.com/TechSmith/mp4v2/commit/f09cceeee5bd7f783fd31f10e8b3c440ccf4c743
    https://github.com/sergiomb2/libmp4v2/commit/3410bc66fb91f46325ab1d008b6a421dd8240949
  - CVE-2018-14325: In MP4v2 2.0.0, there is an integer underflow (with
    resultant memory corruption) when parsing MP4Atom in mp4atom.cpp.
    Fixed by
    https://github.com/TechSmith/mp4v2/commit/e475013c6ef78093055a02b0d035eda0f9f01451
    https://github.com/sergiomb2/libmp4v2/commit/9084868fd9f86bee118001c23171e832f15009f4 (second file)
  - CVE-2018-14326: In MP4v2 2.0.0, there is an integer overflow (with
    resultant memory corruption) when resizing MP4Array for the ftyp
    atom in mp4array.h.
    Fixed by
    https://github.com/TechSmith/mp4v2/commit/70d823ccd8e2d7d0ed9e62fb7e8983d21e6acbeb
    https://github.com/sergiomb2/libmp4v2/commit/9084868fd9f86bee118001c23171e832f15009f4 (first file)
  - CVE-2018-14379: MP4Atom::factory in mp4atom.cpp in MP4v2 2.0.0
    incorrectly uses the MP4ItemAtom data type in a certain case where
    MP4DataAtom is required, which allows remote attackers to cause a
    denial of service (memory corruption) or possibly have unspecified
    other impact via a crafted MP4 file, because access to the data
    structure has different expectations about layout as a result of
    this type confusion.
    Fixed by
    https://github.com/TechSmith/mp4v2/commit/73f38b4296aeb38617fa3923018bb78671c3b833
    https://github.com/sergiomb2/libmp4v2/commit/bb920de948c85e3db4a52292ac7250a50e3bfc86
  - CVE-2018-14403: MP4NameFirstMatches in mp4util.cpp in MP4v2 2.0.0
    mishandles substrings of atom names, leading to use of an
    inappropriate data type for associated atoms. The resulting type
    confusion can cause out-of-bounds memory access.
    Fixed by
    https://github.com/TechSmith/mp4v2/commit/51cb6b36f6c8edf9f195d5858eac9ba18b334a16
    https://github.com/sergiomb2/libmp4v2/commit/a94a3372c6ef66a2276cc6cd92f7ec07a9c8bb6b
</pre>

