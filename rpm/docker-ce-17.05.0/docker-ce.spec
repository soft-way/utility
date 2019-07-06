#
# Spec file for docker ce s390
#

Summary:	docker ce
Name:		docker-ce
Version:	17.05.0
Release:	1%{?dist}
Group:		System/Packages
License:	commercial
Source:		%{name}-%{version}.tar.gz
BuildArch:	s390x

%description
Docker ce for Z

%define _docker_homedir /usr

%prep
%setup -q

%build

%install
rm -fr %{buildroot}
mkdir -p %{buildroot}%{_docker_homedir}

cp -rp usr %{buildroot}%{_docker_homedir}

%pre

%post

%preun

%postun

%clean
rm -fr %{buildroot}

%files
%defattr(-, root, root, -)
%{_docker_homedir}/*

%changelog
* Mon Jul 06 2019
- initial release
