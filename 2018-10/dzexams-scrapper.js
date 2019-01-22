/**
 * dzexams-scrapper.js -- 2018-10-20 (early in the morning... 1am- :p)
 * For my lil bro~
 *
 * Extract documents' download links and generate a download script
 * that uses `aria2c` to save docs in appropriate folders.
 * e.g. '1as/sciences-naturelles/sujets_1984.pdf'
 *
 * @todo Show progress (percentage?)
 * @todo Use CLI arguments to set variables (years/modules/types) 
 * @todo Instead of writing a single file, after getting each link, append the download script so that errors won't screw everything.
 * @todo Get rid of download script -- call aria2c directly from Node.
 * @todo Don't do any of the above.
 *
 */

const SITE = 'https://www.dzexams.com'

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
//const sleep = require('sleep-promise')

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
  const commands = [] // cmd commands
  
  console.log('\n**************************'  )
  console.log(  '* A Dzexams.com scrapper *'  )
  console.log(  '**************************\n')

  for (const year of years) {
    console.log(`## ${year}`)
    for (const module of modules) {
      console.log(`### ${module}`)
      const module_url = `${SITE}/ar/${year}/${module}`
      const module_html = await getHTML(module_url)
      const pages_urls = cheerio.load(module_html)
      ('a.list-group-item')
      .map( (i, a) => {
        const href = a.attribs.href
        if (types.some( type => href.includes(type) ))
          return SITE + href // full url = `origin` (SITE) + `pathname` (href)
        else
          return null
      })
      .get()
      
      console.log(`Found ${pages_urls.length} pages`)
      for (const page_url of pages_urls) {
        console.log('Getting: ' + page_url)
        const html = await getHTML(page_url)
        const pdf_url = cheerio.load(html)('a[download]').attr('href')
        // 'https://www.dzexams.com/ar/sujets/11010' -> 'sujets_11010'
        const type_id = page_url.slice(`${SITE}/ar/`.length).replace('/', '_')
        const file = `${year}/${module}/${type_id}.pdf`
        const command = `aria2c "${pdf_url}" -o "${file}"`
        console.log(`Adding: ${file}`)
        commands.push(command)
        // Sleep for some time before getting the next link?
        //await sleep(1)
      }
      
    }
  }

  fs.writeFileSync('download.cmd', commands.join('\r\n'), 'utf8')
  // Then run `download.cmd` ... manually :p
  
}

work()
