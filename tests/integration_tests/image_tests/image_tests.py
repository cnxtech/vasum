'''! Module used to test containers' images completeness

@author: Michal Witanowski (m.witanowski@samsung.com)
'''
import unittest
from sc_integration_tests.common import sc_test_utils
from config_checker import *
import xml.etree.ElementTree as ET


# security-containers daemon user name and user ID
SCS_USER_NAME = "security-containers"
SCS_UID = 377

DAEMON_DBUS_SOCKET_NAME = "org.tizen.containers.domain"

# dbus config file path relative to container's root
DBUS_CONFIG_PATH = "etc/dbus-1/system.d/" + DAEMON_DBUS_SOCKET_NAME + ".conf"

# main daemon config
DAEMON_CONFIG_PATH = "/etc/security-containers/daemon.conf"


class ContainerImageTestCase(unittest.TestCase):
    '''! Test case class verifying containers' images
    '''

    @classmethod
    def setUpClass(self):
        '''! Sets up testing environment - collects container names and paths.
        '''
        self.configChecker = ConfigChecker(DAEMON_CONFIG_PATH)

    def test01_scsUserExistence(self):
        '''! Verifies if "security-containers" user with an appropriate UID exists within the
             containers.
        '''
        for containerName, containerPath in self.configChecker.containers.iteritems():
            # chroot into a container and get UID of the user
            output, ret = sc_test_utils.launchProc("chroot " + containerPath +
                                                   " /usr/bin/id -u " + SCS_USER_NAME)

            self.assertEqual(ret, 0, "User '" + SCS_USER_NAME + "' does not exist in '" +
                             containerName + "' container.")

            # cast to integer to remove white spaces, etc.
            uid = int(output)
            self.assertEqual(uid, SCS_UID, "Invalid UID of '" + SCS_USER_NAME + "' in '" +
                             containerName + "' container: got " + str(uid) +
                             ", should be " + str(SCS_UID))

    def test02_dbusConfig(self):
        '''! Verifies if dbus configuration file exists within containers.
        '''
        for containerName, containerPath in self.configChecker.containers.iteritems():
            configPath = os.path.join(containerPath, DBUS_CONFIG_PATH)
            self.assertTrue(os.path.isfile(configPath), "Dbus configuration not found in '" +
                            containerName + "' container")

            tree = ET.parse(configPath)
            root = tree.getroot()
            self.assertEqual(root.tag, "busconfig", "Invalid root node name")

            # validate "security-containers" access to the dbus
            ownCheck = False
            sendDestinationCheck = False
            receiveSenderCheck = False

            # extract directory mountings
            for elem in root.iterfind("policy"):
                if "user" not in elem.attrib:
                    continue

                self.assertEqual(elem.attrib["user"], SCS_USER_NAME, "dbus configuration allows '" +
                    elem.attrib["user"] + "' user to access the dbus socket.")

                for allowNode in elem.iterfind("allow"):
                    if "own" in allowNode.attrib:
                        ownCheck = (allowNode.attrib["own"] == DAEMON_DBUS_SOCKET_NAME)
                    if "send_destination" in allowNode.attrib:
                        sendDestinationCheck = (allowNode.attrib["send_destination"] ==
                            DAEMON_DBUS_SOCKET_NAME)
                    if "receive_sender" in allowNode.attrib:
                        receiveSenderCheck = (allowNode.attrib["receive_sender"] ==
                            DAEMON_DBUS_SOCKET_NAME)

            if not (ownCheck and sendDestinationCheck and sendDestinationCheck):
                raise Exception("Invalid dbus configuration in '" + containerName + "' container")