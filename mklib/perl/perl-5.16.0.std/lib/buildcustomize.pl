#!perl

# We are miniperl, building extensions
# Reset @INC completely, adding the directories we need, and removing the
# installed directories (which we don't need to read, and may confuse us)
@INC = (q /home/forge/workspaces/jerl_google/jerl_home/mklib/perl/perl-5.16.0.std/perl-5.16.0/cpan/AutoLoader/lib ,
        q /home/forge/workspaces/jerl_google/jerl_home/mklib/perl/perl-5.16.0.std/perl-5.16.0/dist/Carp/lib ,
        q /home/forge/workspaces/jerl_google/jerl_home/mklib/perl/perl-5.16.0.std/perl-5.16.0/dist/Cwd ,
        q /home/forge/workspaces/jerl_google/jerl_home/mklib/perl/perl-5.16.0.std/perl-5.16.0/dist/Cwd/lib ,
        q /home/forge/workspaces/jerl_google/jerl_home/mklib/perl/perl-5.16.0.std/perl-5.16.0/dist/ExtUtils-Command/lib ,
        q /home/forge/workspaces/jerl_google/jerl_home/mklib/perl/perl-5.16.0.std/perl-5.16.0/dist/ExtUtils-Install/lib ,
        q /home/forge/workspaces/jerl_google/jerl_home/mklib/perl/perl-5.16.0.std/perl-5.16.0/cpan/ExtUtils-MakeMaker/lib ,
        q /home/forge/workspaces/jerl_google/jerl_home/mklib/perl/perl-5.16.0.std/perl-5.16.0/dist/ExtUtils-Manifest/lib ,
        q /home/forge/workspaces/jerl_google/jerl_home/mklib/perl/perl-5.16.0.std/perl-5.16.0/cpan/File-Path/lib ,
        q /home/forge/workspaces/jerl_google/jerl_home/mklib/perl/perl-5.16.0.std/perl-5.16.0/ext/re ,
        q /home/forge/workspaces/jerl_google/jerl_home/mklib/perl/perl-5.16.0.std/perl-5.16.0/dist/Term-ReadLine/lib ,
        q /home/forge/workspaces/jerl_google/jerl_home/mklib/perl/perl-5.16.0.std/perl-5.16.0/lib ,
        q . );
