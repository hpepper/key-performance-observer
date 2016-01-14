package {'autoconf': ensure => present }
package {'eclipse-cdt': ensure => present }

# required by the se2graphite.pl
package {'libxml-simple-perl': ensure => present }


