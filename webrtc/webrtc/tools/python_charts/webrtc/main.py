#!/usr/bin/env python
#  Copyright (c) 2011 The WebRTC project authors. All Rights Reserved.
#
#  Use of this source code is governed by a BSD-style license
#  that can be found in the LICENSE file in the root of the source
#  tree. An additional intellectual property rights grant can be found
#  in the file PATENTS.  All contributing project authors may
#  be found in the AUTHORS file in the root of the source tree.

import os
import gviz_api
import webrtc.data_helper

def main():
  """
  This Python script displays a web page with test created with the
  video_quality_measurement program, which is a tool in WebRTC.

  The script requires on two external files and one Python library:
  - A HTML template file with layout and references to the json variables
    defined in this script
  - A data file in Python format, containing the following:
    - test_configuration - a dictionary of test configuration names and values.
    - frame_data_types - a dictionary that maps the different metrics to their
      data types.
    - frame_data - a list of dictionaries where each dictionary maps a metric to
      it's value.
  - The gviz_api.py of the Google Visualization Python API, available at
    http://code.google.com/p/google-visualization-python/

  The HTML file is shipped with the script, while the data file must be
  generated by running video_quality_measurement with the --python flag
  specified.
  """
  print 'Content-type: text/html\n' # the newline is required!

  page_template_filename = '../templates/chart_page_template.html'
  # The data files must be located in the project tree for app engine being
  # able to access them.
  data_filenames = [ '../data/vp8_sw.py', '../data/vp8_hw.py' ]
  # Will contain info/error messages to be displayed on the resulting page.
  messages = []
  # Load the page HTML template.
  try:
    f = open(page_template_filename)
    page_template = f.read()
    f.close()
  except IOError as e:
    ShowErrorPage('Cannot open page template file: %s<br>Details: %s' %
                  (page_template_filename, e))
    return

  # Read data from external Python script files. First check that they exist.
  for filename in data_filenames:
    if not os.path.exists(filename):
      messages.append('Cannot open data file: %s' % filename)
      data_filenames.remove(filename)

  # Read data from all existing input files.
  data_list = []
  test_configurations = []
  names = []

  for filename in data_filenames:
    read_vars = {} # empty dictionary to load the data into.
    execfile(filename, read_vars, read_vars)

    test_configuration = read_vars['test_configuration']
    table_description = read_vars['frame_data_types']
    table_data = read_vars['frame_data']

    # Verify the data in the file loaded properly.
    if not table_description or not table_data:
      messages.append('Invalid input file: %s. Missing description list or '
                      'data dictionary variables.' % filename)
      continue

    # Frame numbers appear as number type in the data, but Chart API requires
    # values of the X-axis to be of string type.
    # Change the frame_number column data type:
    table_description['frame_number'] = ('string', 'Frame number')
    # Convert all the values to string types:
    for row in table_data:
      row['frame_number'] = str(row['frame_number'])

    # Store the unique data from this file in the high level lists.
    test_configurations.append(test_configuration)
    data_list.append(table_data)
    # Name of the test run must be present.
    test_name = FindConfiguration(test_configuration, 'name')
    if not test_name:
      messages.append('Invalid input file: %s. Missing configuration key '
                      '"name"', filename)
      continue
    names.append(test_name)

  # Create data helper and build data tables for each graph.
  helper = webrtc.data_helper.DataHelper(data_list, table_description,
                                         names, messages)

  # Loading it into gviz_api.DataTable objects and create JSON strings.
  description, data = helper.CreateConfigurationTable(test_configurations)
  configurations = gviz_api.DataTable(description, data)
  json_configurations = configurations.ToJSon()  # pylint: disable=W0612

  description, data = helper.CreateData('ssim')
  ssim = gviz_api.DataTable(description, data)
  # pylint: disable=W0612
  json_ssim_data = ssim.ToJSon(helper.GetOrdering(description))

  description, data = helper.CreateData('psnr')
  psnr = gviz_api.DataTable(description, data)
  # pylint: disable=W0612
  json_psnr_data = psnr.ToJSon(helper.GetOrdering(description))

  description, data = helper.CreateData('packets_dropped')
  packet_loss = gviz_api.DataTable(description, data)
  # pylint: disable=W0612
  json_packet_loss_data = packet_loss.ToJSon(helper.GetOrdering(description))

  description, data = helper.CreateData('bit_rate')
  # Add a column of data points for the desired bit rate to be plotted.
  # (uses test configuration from the last data set, assuming it is the same
  # for all of them)
  desired_bit_rate = FindConfiguration(test_configuration, 'bit_rate_in_kbps')
  if not desired_bit_rate:
    ShowErrorPage('Cannot configuration field named "bit_rate_in_kbps"')
    return
  desired_bit_rate = int(desired_bit_rate)
  # Add new column data type description.
  description['desired_bit_rate'] = ('number', 'Desired bit rate (kbps)')
  for row in data:
    row['desired_bit_rate'] = desired_bit_rate
  bit_rate = gviz_api.DataTable(description, data)
  # pylint: disable=W0612
  json_bit_rate_data = bit_rate.ToJSon(helper.GetOrdering(description))

  # Format the messages list with newlines.
  messages = '\n'.join(messages)

  # Put the variables as JSon strings into the template.
  print page_template % vars()

def FindConfiguration(configuration, name):
  """ Finds a configuration value using it's name.
      Returns the first configuration with a matching name. Returns None if no
      matching configuration is found. """
  return_value = None
  for row in configuration:
    if row['name'] == name:
      return_value = row['value']
      break
  return return_value

def ShowErrorPage(error_message):
  print '<html><body>%s</body></html>' % error_message

if __name__ == '__main__':
  main()
