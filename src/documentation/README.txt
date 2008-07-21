This is the base documentation directory.

skinconf.xml     # This file customizes Forrest for your project. In it, you
                 # tell forrest the project name, logo, copyright info, etc

sitemap.xmap     # Optional. This sitemap is consulted before all core sitemaps.
                 # See http://forrest.apache.org/docs/project-sitemap.html



Using (you need JAVA 5 !!!):

export FORREST_HOME=?
export PATH=$PATH:$FORREST_HOME/bin

forrest site
