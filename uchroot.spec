Summary:	Run command with special root directory and specified account's uid and gid
Summary(pl.UTF-8):   Uruchamia polecenie z podanym katalogiem głównym, użytkownikiem i grupą
Name:		uchroot
Version:	0.010730
Release:	1
License:	GPL
Group:		Applications/System
Source0:	ftp://ftp.corpit.ru/pub/avcheck/%{name}.c
# Source0-md5:	613d0f5c511af145d4f754d749950338
BuildRoot:	%{tmpdir}/%{name}-%{version}-root-%(id -u -n)

%description
Runs another program under a specified account's uid and gid with root
directory set to NEWROOT.

%description -l pl.UTF-8
Uruchamia polecenie lub powłokę interaktywną z podanym katalogiem
głównym, użytkownikiem i grupą.

%prep
%setup -q -T -c
cp -f %{SOURCE0} .

%build
%{__cc} %{rpmcflags} %{rpmldflags} -o uchroot uchroot.c

%install
rm -rf $RPM_BUILD_ROOT
install -D uchroot $RPM_BUILD_ROOT%{_sbindir}/uchroot

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(644,root,root,755)
%attr(755,root,root) %{_sbindir}/*
