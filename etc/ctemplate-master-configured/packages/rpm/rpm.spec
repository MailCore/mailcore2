%define	RELEASE	1
%define rel     %{?CUSTOM_RELEASE} %{!?CUSTOM_RELEASE:%RELEASE}
%define	prefix	/usr

Name: %NAME
Summary: Simple but powerful template language for C++
Version: %VERSION
Release: %rel
Group: Development/Libraries
URL: http://code.google.com/p/ctemplate
License: BSD
Vendor: Google Inc. and others
Packager: Google Inc. and others <google-ctemplate@googlegroups.com>
Source: http://%{NAME}.googlecode.com/files/%{NAME}-%{VERSION}.tar.gz
Distribution: Redhat 7 and above.
Buildroot: %{_tmppath}/%{name}-root
Prefix: %prefix

%description
The %name package contains a library implementing a simple but
powerful template language for C++.  It emphasizes separating logic
from presentation: it is impossible to embed application logic in this
template language.  This limits the power of the template language
without limiting the power of the template *system*.  Indeed, Google's
"main" web search uses this system exclusively for formatting output.

%package devel
Summary: Simple but powerful template language for C++
Group: Development/Libraries
Requires: %{NAME} = %{VERSION}

%description devel
The %name-devel package contains static and debug libraries and header
files for developing applications that use the %name package.

%changelog
	* Wed Apr 22 2009  <opensource@google.com>
	- Change build rule to use %configure instead of ./configure
	- Change install to use DESTDIR instead of prefix for make install
	- Use wildcards for doc/ and lib/ directories
        - Use {_libdir}/{_includedir}/etc instead of {prefix}/lib, etc

	* Mon Mar 13 2006 <opensource@google.com>
	- First draft

%prep
%setup

%build
# I can't use '% configure', because it defines -m32 which breaks on
# my development environment for some reason.  But I do take
# as much from % configure (in /usr/lib/rpm/macros) as I can.
./configure --prefix=%{_prefix} --exec-prefix=%{_exec_prefix} --bindir=%{_bindir} --sbindir=%{_sbindir} --sysconfdir=%{_sysconfdir} --datadir=%{_datadir} --includedir=%{_includedir} --libdir=%{_libdir} --libexecdir=%{_libexecdir} --localstatedir=%{_localstatedir} --sharedstatedir=%{_sharedstatedir} --mandir=%{_mandir} --infodir=%{_infodir}
make

%install
rm -rf $RPM_BUILD_ROOT
make DESTDIR=$RPM_BUILD_ROOT install

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)

%docdir %{prefix}/share/doc/%{NAME}-%{VERSION}
%{prefix}/share/doc/%{NAME}-%{VERSION}/*

%{_libdir}/*.so.*

%files devel
%defattr(-,root,root)

%{_includedir}/ctemplate
%{_libdir}/*.a
%{_libdir}/*.la
%{_libdir}/*.so
%{_bindir}/make_tpl_varnames_h
%{_bindir}/template-converter
%{_bindir}/diff_tpl_auto_escape
%{_libdir}/pkgconfig/*.pc
