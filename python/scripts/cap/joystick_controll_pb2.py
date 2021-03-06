# -*- coding: utf-8 -*-
# Generated by the protocol buffer compiler.  DO NOT EDIT!
# source: joystick_controll.proto
"""Generated protocol buffer code."""
from google.protobuf import descriptor as _descriptor
from google.protobuf import message as _message
from google.protobuf import reflection as _reflection
from google.protobuf import symbol_database as _symbol_database
# @@protoc_insertion_point(imports)

_sym_db = _symbol_database.Default()




DESCRIPTOR = _descriptor.FileDescriptor(
  name='joystick_controll.proto',
  package='joystick_controll',
  syntax='proto2',
  serialized_options=None,
  create_key=_descriptor._internal_create_key,
  serialized_pb=b'\n\x17joystick_controll.proto\x12\x11joystick_controll\"\xeb\x01\n\x0cJoystickType\x12\x15\n\rcancel_motion\x18\x08 \x01(\x11\x12\x0e\n\x06motion\x18\x01 \x01(\x11\x12\x11\n\twalk_step\x18\x02 \x01(\x11\x12\x12\n\nwalk_angle\x18\x03 \x01(\x11\x12\x15\n\rwalk_x_stride\x18\x04 \x01(\x11\x12\x13\n\x0bwalk_period\x18\x05 \x01(\x11\x12\x15\n\rwalk_y_stride\x18\x06 \x01(\x11\x12\x0c\n\x04mode\x18\x07 \x01(\x11\"$\n\nModeNumber\x12\x06\n\x02GC\x10\x00\x12\x06\n\x02PK\x10\x01\x12\x06\n\x02GK\x10\x02\"\x16\n\tRobotPort\x12\t\n\x04port\x10\xbf>'
)



_JOYSTICKTYPE_MODENUMBER = _descriptor.EnumDescriptor(
  name='ModeNumber',
  full_name='joystick_controll.JoystickType.ModeNumber',
  filename=None,
  file=DESCRIPTOR,
  create_key=_descriptor._internal_create_key,
  values=[
    _descriptor.EnumValueDescriptor(
      name='GC', index=0, number=0,
      serialized_options=None,
      type=None,
      create_key=_descriptor._internal_create_key),
    _descriptor.EnumValueDescriptor(
      name='PK', index=1, number=1,
      serialized_options=None,
      type=None,
      create_key=_descriptor._internal_create_key),
    _descriptor.EnumValueDescriptor(
      name='GK', index=2, number=2,
      serialized_options=None,
      type=None,
      create_key=_descriptor._internal_create_key),
  ],
  containing_type=None,
  serialized_options=None,
  serialized_start=222,
  serialized_end=258,
)
_sym_db.RegisterEnumDescriptor(_JOYSTICKTYPE_MODENUMBER)

_JOYSTICKTYPE_ROBOTPORT = _descriptor.EnumDescriptor(
  name='RobotPort',
  full_name='joystick_controll.JoystickType.RobotPort',
  filename=None,
  file=DESCRIPTOR,
  create_key=_descriptor._internal_create_key,
  values=[
    _descriptor.EnumValueDescriptor(
      name='port', index=0, number=7999,
      serialized_options=None,
      type=None,
      create_key=_descriptor._internal_create_key),
  ],
  containing_type=None,
  serialized_options=None,
  serialized_start=260,
  serialized_end=282,
)
_sym_db.RegisterEnumDescriptor(_JOYSTICKTYPE_ROBOTPORT)


_JOYSTICKTYPE = _descriptor.Descriptor(
  name='JoystickType',
  full_name='joystick_controll.JoystickType',
  filename=None,
  file=DESCRIPTOR,
  containing_type=None,
  create_key=_descriptor._internal_create_key,
  fields=[
    _descriptor.FieldDescriptor(
      name='cancel_motion', full_name='joystick_controll.JoystickType.cancel_motion', index=0,
      number=8, type=17, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='motion', full_name='joystick_controll.JoystickType.motion', index=1,
      number=1, type=17, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='walk_step', full_name='joystick_controll.JoystickType.walk_step', index=2,
      number=2, type=17, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='walk_angle', full_name='joystick_controll.JoystickType.walk_angle', index=3,
      number=3, type=17, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='walk_x_stride', full_name='joystick_controll.JoystickType.walk_x_stride', index=4,
      number=4, type=17, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='walk_period', full_name='joystick_controll.JoystickType.walk_period', index=5,
      number=5, type=17, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='walk_y_stride', full_name='joystick_controll.JoystickType.walk_y_stride', index=6,
      number=6, type=17, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
    _descriptor.FieldDescriptor(
      name='mode', full_name='joystick_controll.JoystickType.mode', index=7,
      number=7, type=17, cpp_type=1, label=1,
      has_default_value=False, default_value=0,
      message_type=None, enum_type=None, containing_type=None,
      is_extension=False, extension_scope=None,
      serialized_options=None, file=DESCRIPTOR,  create_key=_descriptor._internal_create_key),
  ],
  extensions=[
  ],
  nested_types=[],
  enum_types=[
    _JOYSTICKTYPE_MODENUMBER,
    _JOYSTICKTYPE_ROBOTPORT,
  ],
  serialized_options=None,
  is_extendable=False,
  syntax='proto2',
  extension_ranges=[],
  oneofs=[
  ],
  serialized_start=47,
  serialized_end=282,
)

_JOYSTICKTYPE_MODENUMBER.containing_type = _JOYSTICKTYPE
_JOYSTICKTYPE_ROBOTPORT.containing_type = _JOYSTICKTYPE
DESCRIPTOR.message_types_by_name['JoystickType'] = _JOYSTICKTYPE
_sym_db.RegisterFileDescriptor(DESCRIPTOR)

JoystickType = _reflection.GeneratedProtocolMessageType('JoystickType', (_message.Message,), {
  'DESCRIPTOR' : _JOYSTICKTYPE,
  '__module__' : 'joystick_controll_pb2'
  # @@protoc_insertion_point(class_scope:joystick_controll.JoystickType)
  })
_sym_db.RegisterMessage(JoystickType)


# @@protoc_insertion_point(module_scope)
