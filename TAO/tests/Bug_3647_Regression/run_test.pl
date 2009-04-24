eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# $Id$
# -*- perl -*-

use lib "$ENV{ACE_ROOT}/bin";
use PerlACE::TestTarget;
use strict;

my $verbose = '';

foreach my $i (@ARGV) {
    if ($i eq '-verbose') {
        $verbose = ' -v';
    }
}

my $backend = PerlACE::TestTarget::create_target(1)
  or die "Create target 1 failed\n";
my $middle = PerlACE::TestTarget::create_target(2)
  or die "Create target 2 failed\n";
my $client = PerlACE::TestTarget::create_target(3)
  or die "Create target 3 failed\n";

my $backend_ior = "backend.ior";
my $middle_ior = "middle.ior";

my $backend_iorfile = $backend->LocalFile ($backend_ior);
my $middle_in_iorfile = $middle->LocalFile ($backend_ior);
my $middle_out_iorfile = $middle->LocalFile ($middle_ior);
my $client_in_iorfile = $client->LocalFile ($middle_ior);

$backend->DeleteFile($backend_ior);
$middle->DeleteFile($backend_ior);
$middle->DeleteFile($middle_ior);
$client->DeleteFile($middle_ior);

my $BE =
  $backend->CreateProcess ("backend_server",
			   " -o $backend_iorfile"
			   . $verbose);
my $MD =
  $middle->CreateProcess ("/usr/bin/strace"," -o md.strace.txt ./middle_server".
			  " -s DELAYED -t 300 "
			  ." -o $middle_out_iorfile"
			  . $verbose
			  . " -k file://$middle_in_iorfile");
my $CL = $client->CreateProcess ("client",
				 " -k file://$client_in_iorfile"
				 ." -t 600 "
				 .$verbose);
my $be_status = $BE->Spawn ();
if ($be_status != 0) {
    print STDERR "ERROR: server returned $be_status\n";
    exit 1;
}

if ($backend->WaitForFileTimed ($backend_ior,
				$backend->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$backend_iorfile>\n";
    $BE->Kill (); $BE->TimedWait (1);
    exit 1;
}

if ($backend->GetFile ($backend_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$backend_iorfile>\n";
    $BE->Kill (); $BE->TimedWait (1);
    exit 1;
}

if ($middle->PutFile ($backend_ior) == -1) {
    print STDERR "ERROR: cannot set file <$middle_in_iorfile>\n";
    $BE->Kill (); $BE->TimedWait (1);
    exit 1;
}

my $md_status = $MD->Spawn ();
if ($md_status != 0) {
    print STDERR "ERROR: server returned $md_status\n";
    $BE->Kill (); $BE->TimedWait (1);
    exit 1;
}

if ($middle->WaitForFileTimed ($middle_ior,
			       $middle->ProcessStartWaitInterval()) == -1) {
    print STDERR "ERROR: cannot find file <$middle_in_iorfile>\n";
    $MD->Kill (); $MD->TimedWait (1);
    $BE->Kill (); $BE->TimedWait (1);
    exit 1;
}

if ($middle->GetFile ($middle_ior) == -1) {
    print STDERR "ERROR: cannot retrieve file <$middle_out_iorfile>\n";
    $MD->Kill (); $MD->TimedWait (1);
    $BE->Kill (); $BE->TimedWait (1);
    exit 1;
}

if ($client->PutFile ($middle_ior) == -1) {
    print STDERR "ERROR: cannot set file <$client_in_iorfile>\n";
    $MD->Kill (); $MD->TimedWait (1);
    $BE->Kill (); $BE->TimedWait (1);
    exit 1;
}

my $status = 0;
my $client_status =
  $CL->SpawnWaitKill ($client->ProcessStartWaitInterval());
if ($client_status != 0) {
    print STDERR "ERROR: client returned $client_status\n";
    $status = 1;
}

$md_status = $MD->WaitKill ($middle->ProcessStopWaitInterval());
if ($md_status != 0) {
    print STDERR "ERROR: middle returned $md_status\n";
    $status = 1;
}

$be_status = $BE->WaitKill ($backend->ProcessStopWaitInterval());
if ($be_status != 0) {
    print STDERR "ERROR: backend returned $be_status\n";
    $status = 1;
}

$backend->DeleteFile($backend_ior);
$middle->DeleteFile($backend_ior);
$middle->DeleteFile($middle_ior);
$client->DeleteFile($middle_ior);

exit $status;
