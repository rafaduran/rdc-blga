

Binary local genetic algorithm (BLGA) is a GA that perform a local search using a niching technique. This library; originally created by Carlos García Martinez, teacher in UCO univesity (Córdoba, Spain), was expanded during my final year project to test global optimization capabilities.

After finishing my final year project, I spent some time trying to improve even more global optimization and now I'm trying to expand the project, not BLGA itself but adding support to many features missing during my my final year project, like GUI and remote access support.

# Objectives #

As described above one of the main focus on this project won't be improve BLGA library but giving to it support to many features improving usability, availability, decoupling of external tools (e.g.: chart tools), ... All this features will improve both user and developer experience, getting a new and improved set of tools. However, this project is just a hobby and the main objective is improving my own programming skills.

# Requirements #

I'm following [Unified Process(UP)](http://en.wikipedia.org/wiki/Unified_Process) style methodology, so requirements will be updated and refined iteratively and the process will be focus on code. Despite I'm using UP methodologies use cases I won't focus on use cases, due to project characteristics. At this moment I can distinguish four main blocks of requirements, that will be three milestones:

  * Improve BLGA legacy code
  * Add GUI support
  * Add remote access control, via web
  * Remove some external dependencies, adding support to needed features

Further information on [milestones page](Milestones.md).

### First iteration ###

As result of first iteration some tests have been added using [googletest framework](http://code.google.com/p/googletest/), source code documentation has been written using [sphinx](http://sphinx.pocoo.org/) and outputting has been improved, using now JSON output as default. This will be very useful on next iteration since [python](http://www.python.org/) has great support to JSON format, and thus JSON files will be used to export Blga results to new python modules from the next iterations. As UP recommends, using CASE tool ([BOUML](http://bouml.free.fr/)) I've got reverse UML class diagram:

![http://rdc-blga.googlecode.com/git/docs/images/class_diagram.jpeg](http://rdc-blga.googlecode.com/git/docs/images/class_diagram.jpeg)

Neither attributes nor methods are shown since you can check them on source code documentation and not showing them improves readability.

<u><b>Note:</b></u> Class diagram is not as exact as I would like since most of `Blga` relations are inherited from `LocalSearcher` and are used via `LocalSearcher` pointer, so they should point to `LocalSearcher` not `Blga`.

### Source code docs ###

Source code documentation can be found at [this link](http://rdc-blga.appspot.com/html/index.html), running under [Google App Engine - Google Code](http://code.google.com/intl/es-ES/appengine/)

### Git branching model ###

From blga1 and beyond I'm using a new Git branching model:

![http://rdc-blga.googlecode.com/git/docs/images/git-branching.png](http://rdc-blga.googlecode.com/git/docs/images/git-branching.png)