/**
 * DzExams-renamer.js -- 2018-11-23
 *
 * My brother was like: Why the hell did you download all branches' subjects?
 * And I was like: Grrrr fine, I'll just mark relevant ones~
 * Example: 'sujets_66516.pdf' -> '@sujets_66516.pdf'
 */
 
const ORIGIN = 'https://www.dzexams.com'

const branch = 'ج.م.ع.ت'
const years = ['1as']
const modules = [
  'sciences-naturelles', 'physique', 'mathematiques',
  'informatique', 'technologie-st', 'histoire-geographie',
  'francais', 'anglais', 'arabe',
  'tarbia-islamia',
  //'tamazight',
  ]
  
const types = [
  'sujets',
  // 'documents',
  ]

// should come first (before "variables")
const fs = require('fs')
const axios = require('axios')
const cheerio = require('cheerio')

async function getHTML(url) {
  try {
    const res = await axios.get(url)
    return res.data
  } catch(e) {
    //console.log(e)
    console.log('Err getting ' + url)
    return ''
  }
}

async function work() {
  console.log('\n**************************'  )
  console.log(  '* A Dzexams.com renamer  *'  )
  console.log(  '**************************\n')

  const cmds = []
  
  for (const year of years) {
    console.log('\n' + year)
    console.log('=======')
    for (const module of modules) {
      console.log('\n' + module)
      console.log('-'.repeat(module.length))
      console.log('Working...')
      const module_url = `${ORIGIN}/ar/${year}/${module}`
      const module_html = await getHTML(module_url)
      const pages_urls = cheerio.load(module_html)
      ('a.list-group-item')
      .map( (i, $a) => {
        const href = $a.attribs.href
        if (cheerio($a).text().includes(branch) &&
            types.some(type => href.includes(type))
           )
          return ORIGIN + href // full URL = ORIGIN + `pathname` (href)
        else
          return null
      })
      .get()
      
      console.log(`Found ${pages_urls.length} pages`)
        pages_urls.forEach(page_url => {
        // 'https://www.dzexams.com/ar/sujets/16510' -> 'sujets_16510'
        const type_id = page_url.slice(`${ORIGIN}/ar/`.length).replace('/', '_')
        cmds.push(`ren ${year}/${module}/${type_id}.pdf @${type_id}.pdf`)
      })
    }
  }

  fs.writeFileSync('renamer.cmd', cmds.join('\r\n'), 'utf8')

}

work()
