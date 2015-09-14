{
  'targets': [
    {
      'target_name': 'event-emitter',
      'sources': [ 
        './event-emitter.cc',
      ],
      'include_dirs': [
        '<!(node -e "require(\'nan\')")',
      ],
    }
  ]
}
